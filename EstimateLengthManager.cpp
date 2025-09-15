// EstimateLengthManager.cpp
#include "EstimateLengthManager.h"
#include "EstimateLengthResult.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <cmath>
#include "SettingsManagerA.h"
#include "SettingsManagerB.h"
#include "GroupResult.h"
#include "CommonTypes.h"

EstimateLengthManager::EstimateLengthManager() {}



void EstimateLengthManager::performEstimation() {
    printPerSectionBreakdown();         // Step 1: Print per-section breakdown 
    groupAndOptimizeSections();         // Step 2: Group and optimize sections
}

void EstimateLengthManager::addWindowSections(const std::string& windowLabel, const SectionMap& sections) {
    windowSections[windowLabel] = sections;
}

// ✅ Paste removeWindow here
void EstimateLengthManager::removeWindow(const std::string& label) {
    // Remove the sections data for this window
    auto it = windowSections.find(label);
    if (it != windowSections.end()) {
        windowSections.erase(it);
    }

    // Optionally, also clear from combinedSections if you want immediate removal
    for (auto& [sectionName, items] : combinedSections) {
        items.erase(std::remove_if(items.begin(), items.end(),
                                   [&](const std::pair<std::string, double>& p) {
                                       return p.first.find(label) != std::string::npos;
                                   }),
                    items.end());
    }
}

///////////////////////// Print Per-Section Breakdown /////////////////////////

void EstimateLengthManager::printPerSectionBreakdown() const {
    combinedSections.clear();

    for (const auto& windowPair : windowSections) {
        const std::string& windowLabel = windowPair.first;
        const SectionMap& sections = windowPair.second;

        for (const auto& sectionPair : sections) {
            const std::string& sectionName = sectionPair.first;
            const std::vector<std::pair<double, std::string>>& values = sectionPair.second;

            for (const auto& valPair : values) {
                combinedSections[sectionName].emplace_back(valPair.second, valPair.first); 
            }
        }
    }

    std::cout << "\n================== 🧾 Section-wise Breakdown ==================\n";

    for (const auto& sectionEntry : combinedSections) {
        const std::string& sectionName = sectionEntry.first;
        const std::vector<std::pair<std::string, double>>& items = sectionEntry.second;

        // 📦 Copy and sort
        std::vector<std::pair<std::string, double>> sortedItems = items;

        std::sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b) {
            auto extractWindowNumber = [](const std::string& label) -> int {
                std::size_t pos = label.find("#");
                if (pos != std::string::npos) {
                    std::string numberPart;
                    for (size_t i = pos + 1; i < label.size(); ++i) {
                        if (isdigit(label[i]))
                            numberPart += label[i];
                        else
                            break;
                    }
                    return std::stoi(numberPart);
                }
                return 9999; // fallback if '#' not found
            };

            int winA = extractWindowNumber(a.first);
            int winB = extractWindowNumber(b.first);

            if (winA == winB)
                return a.first < b.first; // sort by label (e.g. HR < WT)
            return winA < winB;
        });

        std::cout << "\nSection " << sectionName << ":\n";
        for (const auto& [label, value] : sortedItems) {
            std::cout << "    " << std::fixed << std::setprecision(1)
                      << value << " ft   (" << label << ")\n";
        }
    }

    std::cout << "\n====================== Length Optimization ========================\n";
}



////////////////////// Group and Optimize Sections //////////////////////////

EstimateLengthResult EstimateLengthManager::groupAndOptimizeSections() {
    using Item = std::pair<std::string, double>; // label, length

    const double EPS         = 1e-6;
    const double MAX_WASTAGE = 1.75; // same as before

    auto clampNegZero = [](double x) {
        if (std::fabs(x) < 1e-9) return 0.0;
        return x;
    };

    auto approxEq = [&](double a, double b) {
        return std::fabs(a - b) <= 1e-6;
    };

    auto inAllowed = [&](double L, const std::vector<double>& allowed) {
        for (double a : allowed) if (approxEq(a, L)) return true;
        return false;
    };

    auto sumLengths = [](const std::vector<Item>& v) {
        double s = 0.0;
        for (auto& it : v) s += it.second;
        return s;
    };

    // Greedy first-fit (original-order) pack for a given stock length L
    auto greedyPackForLength = [&](double L,
                                   const std::vector<Item>& all,
                                   const std::vector<bool>& usedMask) -> std::vector<int> {
        std::vector<int> picked;
        double sum = 0.0;
        for (int i = 0; i < (int)all.size(); ++i) {
            if (usedMask[i]) continue;
            if (sum + all[i].second <= L + EPS) {
                picked.push_back(i);
                sum += all[i].second;
                if (L - sum < EPS) break; // perfect fit
            }
        }
        return picked;
    };

    // Best-fit across allowed lengths under a wastage cap
    auto bestFitUnderCap = [&](const std::vector<Item>& all,
                               const std::vector<bool>& usedMask,
                               const std::vector<double>& allowed,
                               double cap) -> std::pair<double, std::vector<int>> {
        double bestLen = -1.0, bestWaste = 1e9, bestSum = -1.0;
        std::vector<int> bestPick;
        for (double L : allowed) {
            auto pick = greedyPackForLength(L, all, usedMask);
            if (pick.empty()) continue;
            double sum = 0.0;
            for (int idx : pick) sum += all[idx].second;
            double waste = L - sum;
            if (waste < -EPS) continue; // shouldn’t happen
            if (waste <= cap + EPS) {
                // Prefer smaller waste; tie-breaker: larger used sum
                if (waste + 1e-9 < bestWaste || (approxEq(waste, bestWaste) && sum > bestSum + 1e-9)) {
                    bestWaste = waste; bestLen = L; bestSum = sum; bestPick = std::move(pick);
                }
            }
        }
        return {bestLen, bestPick}; // bestLen<0 => none found under cap
    };

    // Best possible even if wastage > cap (for leftovers completion)
    auto bestPossible = [&](const std::vector<Item>& all,
                            const std::vector<bool>& usedMask,
                            const std::vector<double>& allowed) -> std::pair<double, std::vector<int>> {
        double bestLen = -1.0, bestWaste = 1e9, bestSum = -1.0;
        std::vector<int> bestPick;
        for (double L : allowed) {
            auto pick = greedyPackForLength(L, all, usedMask);
            if (pick.empty()) continue;
            double sum = 0.0;
            for (int idx : pick) sum += all[idx].second;
            double waste = L - sum;
            if (waste < -EPS) continue;
            if (waste + 1e-9 < bestWaste || (approxEq(waste, bestWaste) && sum > bestSum + 1e-9)) {
                bestWaste = waste; bestLen = L; bestSum = sum; bestPick = std::move(pick);
            }
        }
        return {bestLen, bestPick}; // bestLen<0 => no piece fits any allowed length
    };

    // -------------------------------------------------------------

    EstimateLengthResult result;
    sectionSummaries.clear();

    SettingsManagerA& settings        = SettingsManagerA::getInstance();
    SettingsManagerB& generalSettings = SettingsManagerB::getInstance();

    if (combinedSections.empty()) {
        std::cout << "No sections available to group and optimize.\n";
        return result;
    }

    for (const auto& sectionEntry : combinedSections) {
        const std::string& sectionName = sectionEntry.first;
        const auto& piecesRaw          = sectionEntry.second; // vector<pair<label, length>>

        // 1) Allowed lengths
        std::vector<double> allowed = settings.getAllowedLengthsForSection(sectionName);
        if (allowed.empty()) {
            std::cerr << "🚫 No allowed stock lengths configured for section " << sectionName << ". Skipping.\n";
            continue;
        }
        std::sort(allowed.begin(), allowed.end(), std::greater<>());

        const double maxAllowed = allowed.front();
        const double minAllowed = allowed.back();

        // 2) Build items & validate no item exceeds maxAllowed
        std::vector<Item> items;
        items.reserve(piecesRaw.size());
        for (const auto& p : piecesRaw) {
            items.emplace_back(p.first, p.second);
        }
        for (const auto& it : items) {
            if (it.second > maxAllowed + EPS) {
                // Single piece longer than max stock -> impossible to cut from one bar: abort loudly
                std::ostringstream oss;
                oss << "Piece '" << it.first << "' length " << it.second
                    << "ft exceeds max allowed stock (" << maxAllowed
                    << "ft) for section " << sectionName << ". Check settings/data.";
                throw std::runtime_error(oss.str());
            }
        }

        std::vector<bool> used(items.size(), false);

        struct Group {
            double stockLen;                 // either an allowed length or (if offcut) the sum of items
            std::vector<Item> chosen;        // label + length
            bool   offcut = false;           // true only for red-zone small remainder
        };
        std::vector<Group> groups;

        // 3) Main packing under wastage cap
        while (true) {
            auto [L, pick] = bestFitUnderCap(items, used, allowed, MAX_WASTAGE);
            if (L < 0.0 || pick.empty()) break;

            Group g; g.stockLen = L;
            for (int idx : pick) { used[idx] = true; g.chosen.push_back(items[idx]); }
            groups.push_back(std::move(g));
        }

        // 4) Collect leftovers
        std::vector<int> leftoversIdx;
        leftoversIdx.reserve(items.size());
        for (int i = 0; i < (int)items.size(); ++i) if (!used[i]) leftoversIdx.push_back(i);

        if (!leftoversIdx.empty()) {
            // total leftover length
            double leftoverSum = 0.0;
            for (int idx : leftoversIdx) leftoverSum += items[idx].second;

            const double redZone = settings.isGroup1(sectionName)
                                   ? generalSettings.getRedZone1()
                                   : generalSettings.getRedZone2();

            // 4A) If really small remainder (< red zone), treat as offcut (no new bar)
            if (leftoverSum < redZone + EPS) {
                Group g; g.offcut = true; g.stockLen = leftoverSum;
                for (int idx : leftoversIdx) { used[idx] = true; g.chosen.push_back(items[idx]); }
                groups.push_back(std::move(g));
            } else {
                // 4B) Fully pack leftovers into allowed bars (no MAX_WASTAGE cap, we finish everything)
                std::vector<bool> used2 = used;
                while (true) {
                    // Are any left?
                    bool anyLeft = false;
                    for (int i = 0; i < (int)items.size(); ++i) if (!used2[i]) { anyLeft = true; break; }
                    if (!anyLeft) break;

                    auto [L2, pick2] = bestPossible(items, used2, allowed);
                    if (L2 < 0.0 || pick2.empty()) {
                        // This means no single leftover piece fits in the smallest allowed length -> should not happen.
                        std::ostringstream oss;
                        oss << "Leftover pieces for " << sectionName
                            << " cannot fit into any allowed stock length. Check lengths.";
                        throw std::runtime_error(oss.str());
                    }
                    Group g; g.stockLen = L2;
                    for (int idx : pick2) { used2[idx] = true; g.chosen.push_back(items[idx]); }
                    groups.push_back(std::move(g));
                }
                // Mark all finally used
                used = std::move(used2);
            }
        }

        // 5) Output + SectionSummary
        double totalLength = 0.0;
        std::vector<double> usedLengths;

        std::cout << "\n" << sectionName << ":\n";
        for (const auto& g : groups) {
            const double usedLen = sumLengths(g.chosen);
            double wastage       = g.offcut ? 0.0 : clampNegZero(g.stockLen - usedLen);

            // Assert: stock length must be allowed OR offcut
            if (!g.offcut && !inAllowed(g.stockLen, allowed)) {
                std::cerr << "🚩 Internal check: group length " << g.stockLen
                          << " isn’t in allowed list for " << sectionName
                          << ". This should never happen.\n";
            }

            // Build joined labels
            std::string joined;
            for (size_t k = 0; k < g.chosen.size(); ++k) {
                joined += g.chosen[k].first + "(" + std::to_string(g.chosen[k].second) + ")";
                if (k + 1 < g.chosen.size()) joined += " + ";
            }

            // Print line
            std::cout << "  " << std::fixed << std::setprecision(1) << g.stockLen
                      << "ft = " << joined
                      << " | Wastage: " << std::setprecision(1) << wastage << "ft\n";

            totalLength += g.stockLen;
            usedLengths.push_back(g.stockLen);
        }

        // Footer
        std::cout << "Total Length for " << sectionName << ": ";
        for (size_t i = 0; i < usedLengths.size(); ++i) {
            std::cout << std::fixed << std::setprecision(1) << usedLengths[i];
            if (i + 1 < usedLengths.size()) std::cout << " + ";
        }
        std::cout << " = " << std::fixed << std::setprecision(1) << totalLength << "ft\n";

        // Save to summaries
        SectionSummary summary;
        summary.sectionName = sectionName;
        summary.usedLengths = usedLengths;
        summary.totalLength = totalLength;
        sectionSummaries.push_back(std::move(summary));
    }

    return result;
}



///////////////////////////// Clear All Data ////////////////////////////

void EstimateLengthManager::clearAllData() {
    windowSections.clear();
    allSections.clear();
    groupResults.clear();
    optimizedSectionSummary.clear();
    sectionSizes.clear();
    combinedSections.clear();
    sectionSummaries.clear();

    // Reset grouped section summaries
    groupedSectionSummaries = EstimateLengthResult();

    // 🟢 Reset the window counter here
    //windowCounter = 0;

    std::cout << "🔄 EstimateLengthManager memory cleared!\n";
}

// --- at the end ---

const std::vector<SectionSummary>& EstimateLengthManager::getSummaries() const {
    return sectionSummaries;
}

std::vector<SectionSummary>& EstimateLengthManager::getSummaries() {
    return sectionSummaries;
}
// Add this at the end of the cpp file
std::vector<SectionSummary>& EstimateLengthManager::getSectionUsage() {
    return sectionSummaries;
}



 