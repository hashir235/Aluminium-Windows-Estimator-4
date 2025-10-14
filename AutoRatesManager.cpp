// AutoRatesManager.cpp
#include "AutoRatesManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include <algorithm>
#include <limits>
#include <cctype>
#include <vector>
#include <string>

// libcurl write callback
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ----------------- Helpers -----------------
string AutoRatesManager::trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

//------------------ Normalize Key -----------------
string AutoRatesManager::normalizeKey(const string &s) {
    // normalize whitespace and uppercase to help matching
    string out;
    bool space = false;
    for (char c : s) {
        if (isspace((unsigned char)c)) {
            if (!space) { out.push_back(' '); space = true; }
        } else {
            out.push_back((char)toupper((unsigned char)c));
            space = false;
        }
    }
    return trim(out);
}

string AutoRatesManager::gaugeStringForChoice(int choice) {
    switch (choice) {
        case 1: return "1.2MM";
        case 2: return "1.6MM";
        case 3: return "2MM";
        default: return "1.2MM";
    }
}

int AutoRatesManager::colorIndex() const {
    int idx = colorChoice - 1;
    if (idx < 0) idx = 0;
    return idx;
}

// ----------------- Constructor -----------------
AutoRatesManager::AutoRatesManager() {
    std::cout << "🔄 Checking for latest rate list (Internet required)...\n";
    // don't force update on construction if you prefer — but we attempt
    (void) updateRateListFromServer(ratesUrl);
}

// ----------------- Network / CSV loading -----------------
bool AutoRatesManager::checkAndUpdateRates() {
    std::cout << "🔄 Checking and updating rates...\n";
    bool success = updateRateListFromServer(ratesUrl);
    if (success) std::cout << "✅ Rates updated from server.\n";
    else std::cout << "⚠️ Could not update rates from server.\n";
    return success;
}

//-------------------- Remote CSV fetch --------------------
bool AutoRatesManager::updateRateListFromServer(const string& url) {
    CURL* curl = nullptr;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "❌ cURL error: " << curl_easy_strerror(res) << "\n";
        curl_global_cleanup();
        return false;
    }

    // parse from readBuffer
    std::istringstream ss(readBuffer);
    bool ok = parseRatesFromCSV(ss);
    curl_global_cleanup();
    return ok;
}

// Local CSV load (fallback)
bool AutoRatesManager::loadRatesFromCSV(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "⚠️ Could not open " << filename << "\n";
        return false;
    }
    return parseRatesFromCSV(file);
}

// ----------------- CSV Parsing -----------------
bool AutoRatesManager::parseRatesFromCSV(istream& input) {
    sectionRates.clear();
    colorHeaders.clear();

    string line;
    // Read header
    if (!std::getline(input, line)) return false;
    // header tokens separated by comma
    std::vector<string> hdrTokens;
    {
        std::stringstream ss(line);
        string tok;
        while (std::getline(ss, tok, ',')) {
            hdrTokens.push_back(trim(tok));
        }
    }
    if (hdrTokens.size() < 2) {
        // need at least section + one color column
        return false;
    }
    // First token is "SECTIONS(...)" or similar; color headers are the rest
    for (size_t i = 1; i < hdrTokens.size(); ++i) {
        colorHeaders.push_back(trim(hdrTokens[i]));
    }

    // parse rows
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        string tok;
        // first token = section name
        if (!std::getline(ss, tok, ',')) continue;
        string section = trim(tok);
        vector<double> rates;
        // read remaining tokens (as many columns as colorHeaders; if fewer, fill 0)
        for (size_t i = 0; i < colorHeaders.size(); ++i) {
            string numtok;
            if (std::getline(ss, numtok, ',')) {
                numtok = trim(numtok);
                if (numtok.empty()) {
                    rates.push_back(0.0);
                } else {
                    try {
                        double v = std::stod(numtok);
                        rates.push_back(v);
                    } catch (...) {
                        // not a number -> push 0
                        rates.push_back(0.0);
                    }
                }
            } else {
                // missing column -> 0
                rates.push_back(0.0);
            }
        }

        // store under normalized key
        string key = normalizeKey(section);
        sectionRates[key] = std::move(rates);
    }

    return !sectionRates.empty();
}

// ----------------- UI -----------------
void AutoRatesManager::selectOptions() {
    // Gauge selection
    std::cout << "\nSelect Aluminium Thickness (Gauge):\n";
    std::cout << "[1] 1.2mm\n[2] 1.6mm\n[3] 2mm\n";
    int t;
    std::cout << "Enter choice: ";
    std::cin >> t;
    if (t < 1 || t > 3) t = 1;
    thicknessChoice = t;

    // Color selection
    std::vector<std::string> colorNames = {"DULL", "H23/PC-RAL", "SAHRA/BRN", "BLACK/MULTI", "WOOD COAT"};
    std::cout << "\nSelect Aluminium Color:\n";
    for (size_t i = 0; i < colorNames.size(); ++i) {
        std::cout << "[" << (i+1) << "] " << colorNames[i] << "\n";
    }
    
    int c;
    std::cout << "Enter choice: ";
    std::cin >> c;
    if (c < 1) c = 1;
    if (c > (int)colorNames.size()) c = (int)colorNames.size();
    colorChoice = c;

    std::cout << "✅ Gauge: " << gaugeStringForChoice(thicknessChoice)
              << " | Color: " << colorNames[colorChoice - 1] << "\n";
}

// void AutoRatesManager::showLoadedRates() const {
//     std::cout << "\n=== Loaded Rate List (" << sectionRates.size() << " rows) ===\n";
//     std::cout << "Colors:";
//     for (size_t i = 0; i < colorHeaders.size(); ++i) std::cout << " [" << (i+1) << "] " << colorHeaders[i];
//     std::cout << "\n";
//     for (const auto& kv : sectionRates) {
//         std::cout << kv.first;
//         for (double v : kv.second) std::cout << " , " << v;
//         std::cout << "\n";
//     }
// }

// void AutoRatesManager::printFinalSummaryTable() const {
//     // debug print of loaded CSV (section -> rates)
//     showLoadedRates();
// }

//-------------------- Get Rate for Section (fixed) --------------------
double AutoRatesManager::getRateForSection(const string& sectionName) const {
    string base = normalizeKey(sectionName); // e.g., "DC30F"
    string gauge = gaugeStringForChoice(thicknessChoice); // e.g., "1.2MM"
    string candidateWithGauge = base + " (" + gauge + ")";

    int idx = colorIndex();

    // 1) Exact match with gauge appended
    auto it = sectionRates.find(candidateWithGauge);
    if (it != sectionRates.end()) {
        const auto &vec = it->second;
        if (idx >= 0 && idx < (int)vec.size()) return vec[idx];
        if (!vec.empty()) return vec[0];
        return 0.0;
    }

    // 2) Maybe sectionName already contains gauge
    string normalizedInput = normalizeKey(sectionName);
    it = sectionRates.find(normalizedInput);
    if (it != sectionRates.end()) {
        const auto &vec = it->second;
        if (idx >= 0 && idx < (int)vec.size()) return vec[idx];
        if (!vec.empty()) return vec[0];
        return 0.0;
    }

    // 3) Starts with base + contains gauge substring
    for (const auto &kv : sectionRates) {
        const string &k = kv.first;
        if (k.find(base + " (") == 0 && k.find(gauge) != string::npos) {
            const auto &vec = kv.second;
            if (idx >= 0 && idx < (int)vec.size()) return vec[idx];
            if (!vec.empty()) return vec[0];
            return 0.0;
        }
    }

    // 4) Fallback: match base name only (ignore gauge)
    vector<const vector<double>*> candidates;
    for (const auto &kv : sectionRates) {
        const string &k = kv.first;
        if (k.find(base) == 0) {
            candidates.push_back(&kv.second);
        }
    }
    if (candidates.size() == 1) {
        const auto &vec = *candidates.front();
        if (idx >= 0 && idx < (int)vec.size()) return vec[idx];
        if (!vec.empty()) return vec[0];
    }

    // 5) Not found -> print warning + return 0.0
    std::cerr << "⚠️ Warning: Section \"" << sectionName << "\" not found in rates. Using 0.0 rate.\n";
    return 0.0;
}

//-------------------- Review and Edit Rates (fixed) --------------------
void AutoRatesManager::reviewAndEditRates(vector<SectionSummary>& sections) {
    if (sections.empty()) {
        std::cout << "⚠️ No sections to review.\n";
        return;
    }

    while (true) {
        std::cout << "\n≡ Enter Rates per ft for Each Section:\n";
        for (size_t i = 0; i < sections.size(); ++i) {
            double rate = getRateForSection(sections[i].sectionName);
            std::cout << (i+1) << ". Rate for section [" << sections[i].sectionName << "] = Rs. " << (int)rate << "\n";
        }
        std::cout << "\nEnter line number to correct a rate (0 = done): ";
        int choice;
        std::cin >> choice;
        if (!std::cin) { // protect against non-number input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "⚠️ Invalid input. Try again.\n";
            continue;
        }
        if (choice == 0) break;
        if (choice < 1 || choice > (int)sections.size()) {
            std::cout << "⚠️ Invalid line number. Try again.\n";
            continue;
        }

        size_t idx = (size_t)(choice - 1);
        std::cout << "Enter new rate for section [" << sections[idx].sectionName << "]: Rs. ";
        double newRate;
        std::cin >> newRate;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "⚠️ Invalid rate. Operation cancelled for this line.\n";
            continue;
        }
        if (newRate < 0) {
            std::cout << "⚠️ Rate cannot be negative. Operation cancelled.\n";
            continue;
        }

                // --- build a key that matches how CSV keys are stored (with gauge) ---
        string base = normalizeKey(sections[idx].sectionName);
        string gauge = gaugeStringForChoice(thicknessChoice); // "1.2MM" etc
        string candidateKey = base + " (" + gauge + ")";     // matches getRateForSection's candidateWithGauge

        int ci = colorIndex(); // zero-based color column index

        // Try to find the exact gauge-appended key first (most common)
        auto it = sectionRates.find(candidateKey);
        if (it != sectionRates.end()) {
            // ensure vector big enough and overwrite the chosen color column
            std::vector<double>& vec = it->second;
            if ((int)vec.size() <= ci) vec.resize(ci + 1, 0.0);
            vec[ci] = newRate;
        } else {
            // If gauge-appended key not found, try normalized base (fallback)
            auto it2 = sectionRates.find(base);
            if (it2 != sectionRates.end()) {
                std::vector<double>& vec = it2->second;
                if ((int)vec.size() <= ci) vec.resize(ci + 1, 0.0);
                vec[ci] = newRate;
            } else {
                // Neither key exists — create the gauge-appended key (preferred)
                std::vector<double> v(ci + 1, 0.0);
                v[ci] = newRate;
                sectionRates.emplace(candidateKey, std::move(v));
            }
        }

        // Update in-memory SectionSummary so immediate display/calculation reflects change
        sections[idx].totalPrice = sections[idx].totalLength * newRate;


        std::cout << "✅ Updated [" << sections[idx].sectionName << "] rate to Rs. " << (int)newRate << "\n";
    }
}

//-------------------- Compute Totals --------------------
double AutoRatesManager::computeAndStoreTotals(vector<SectionSummary>& sections) {
    if (sections.empty()) return 0.0;

    double grandTotal = 0.0;

    // compute totals and update struct
    for (auto &s : sections) {
        double rate = getRateForSection(s.sectionName);
        s.totalPrice = s.totalLength * rate;
        grandTotal += s.totalPrice;
    }

    // print final table (same look/format as your manual manager)
    std::cout << "\n≡ Final Aluminum Section Cost Table\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Section"
              << " | " << std::setw(30) << "Lengths Used"
              << " | " << std::setw(9)  << "Total ft"
              << " | " << std::setw(8)  << "Rate"
              << " | " << "Total Price\n";
    std::cout << "------------|------------------------------|---------|--------|------------\n";

    for (const auto &s : sections) {
        string lengthsStr;
        for (size_t i = 0; i < s.usedLengths.size(); ++i) {
            lengthsStr += std::to_string((int)s.usedLengths[i]) + "ft";
            if (i + 1 < s.usedLengths.size()) lengthsStr += ", ";
        }
        double rate = getRateForSection(s.sectionName);

        std::cout << std::left << std::setw(12) << s.sectionName << " | "
                  << std::setw(30) << lengthsStr << " | "
                  << std::right << std::setw(7) << std::fixed << std::setprecision(1) << s.totalLength << " | "
                  << "Rs. " << std::setw(4) << std::left << (int)rate << " | "
                  << "Rs. " << (int)s.totalPrice << "\n";
    }

    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Grand Total Aluminum Cost = Rs. {" << (int)grandTotal << "}\n";
    std::cout << "-------------------------------------------------------------\n";

    return grandTotal;
}

//-------------------- Generate material_table.txt --------------------

void AutoRatesManager::generateMaterialTxt(const vector<SectionSummary>& sections) {
    ofstream file("Temp_material_table.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not create Temp_material_table.txt\n";
        return;
    }

    double grandTotal = 0.0;

    // ✅ Recalculate totalPrice and grandTotal
    for (auto &s : const_cast<vector<SectionSummary>&>(sections)) {
        double rate = getRateForSection(s.sectionName);
        s.totalPrice = s.totalLength * rate;
        grandTotal += s.totalPrice;
    }

    file << "Final Aluminum Section Cost Table\n";
   // file << "-------------------------------------------------------------\n";
    file << left << setw(12) << "Section"
         << " | " << setw(30) << "Lengths Used"
         << " | " << setw(9) << "Total ft"
         << " | " << setw(8) << "Rate"
         << " | " << "Total Price\n";
   // file << "------------|------------------------------|---------|--------|------------\n";

    for (const auto &s : sections) {
        string lengthsStr;
        for (size_t i = 0; i < s.usedLengths.size(); ++i) {
            lengthsStr += to_string((int)s.usedLengths[i]) + "ft";
            if (i + 1 < s.usedLengths.size()) lengthsStr += ", ";
        }

        double rate = getRateForSection(s.sectionName);

        file << left << setw(12) << s.sectionName << " | "
             << setw(30) << lengthsStr << " | "
             << right << setw(7) << fixed << setprecision(1) << s.totalLength << " | "
             << "Rs. " << setw(4) << left << (int)rate << " | "
             << "Rs. " << (int)s.totalPrice << "\n";
    }

    file << "-------------------------------------------------------------\n";
    file << "Grand Total Aluminum Cost = Rs. {" << (int)grandTotal << "}\n";
    file << "-------------------------------------------------------------\n";

    file.close();
    cout << "✅ Temp_material_table.txt generated successfully!\n";
}




void AutoRatesManager::reset() {
    sectionRates.clear();   // saare loaded rates clear
    colorHeaders.clear();   // color headers reset
    thicknessChoice = 1;    // default gauge (1.2mm)
    colorChoice = 1;        // default color (DULL)
    std::cout << "🔄 AutoRatesManager data cleared!\n";
}

