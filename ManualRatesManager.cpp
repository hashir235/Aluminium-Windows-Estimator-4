#include "ManualRatesManager.h"
#include <iostream>
#include <iomanip>
#include <limits>

ManualRatesManager::ManualRatesManager(std::vector<SectionSummary>& summaries)
    : sections(summaries) {}

// Step 1: User enters rates
void ManualRatesManager::inputRatesFromUser() {
    if (sections.empty()) {
        std::cout << "⚠️ No sections to rate.\n";
        return;
    }

    std::cout << "\n💰 Enter Rates per ft for Each Section:\n";
    int lineNo = 1;
    for (const auto& s : sections) {
        double rate = 0.0;
        while (true) {
            std::cout << lineNo << ". Rate for section [" << s.sectionName << "] = Rs. ";
            if (std::cin >> rate && rate >= 0.0) {
                sectionRates[s.sectionName] = rate;
                break;
            } else {
                std::cout << "❌ Invalid input! Please enter a non-negative number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        ++lineNo;
    }
}

// Step 2: Allow editing by line number (repeatable)
void ManualRatesManager::editRateByLineNumber() {
    if (sections.empty()) return;

    while (true) {
        int lineNo = 0;
        std::cout << "\nEnter line number to correct a rate (0 = done): ";
        if (!(std::cin >> lineNo)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid input.\n";
            continue;
        }
        if (lineNo == 0) break;

        if (lineNo < 1 || lineNo > static_cast<int>(sections.size())) {
            std::cout << "❌ Out of range. Valid: 1.." << sections.size() << "\n";
            continue;
        }

        const auto& s = sections[lineNo - 1];
        double newRate = 0.0;
        while (true) {
            std::cout << "Enter new rate for [" << s.sectionName << "] = Rs. ";
            if (std::cin >> newRate && newRate >= 0.0) {
                sectionRates[s.sectionName] = newRate;
                std::cout << "✅ Rate updated.\n";
                break;
            } else {
                std::cout << "❌ Invalid input! Please enter a non-negative number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

// Step 3: Compute totals & store back into SectionSummary.totalPrice
double ManualRatesManager::computeAndStoreTotals() {
    grand = 0.0;  // reset stored grand each time
    double grandTotal = 0.0;

    for (auto& s : sections) {
        double rate = 0.0;
        auto it = sectionRates.find(s.sectionName);
        if (it != sectionRates.end()) rate = it->second;

        // totalLength unchanged (ft me hi rahe)
        s.totalPrice = s.totalLength * rate;  // ✅ fresh overwrite

        grandTotal += s.totalPrice;
    }

    grand = grandTotal;  // store for later getTotalAluminiumCost()
    return grandTotal;
}



// Step 4: Print final table
void ManualRatesManager::printFinalSummaryTable() const {
    if (sections.empty()) {
        std::cout << "⚠️ Nothing to show.\n";
        return;
    }

    std::cout << "\n💵 Final Aluminum Section Cost Table\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "Section"
              << " | " << std::setw(30) << "Lengths Used"
              << " | " << std::setw(9)  << "Total ft"
              << " | " << std::setw(8)  << "Rate"
              << " | " << "Total Price\n";
    std::cout << "--------|------------------------------|---------|--------|------------\n";

   

    for (const auto& s : sections) {
    double rate = 0.0;
    auto it = sectionRates.find(s.sectionName);
    if (it != sectionRates.end()) rate = it->second;

    // ab direct struct se lo
    double totalPrice = s.totalPrice;

    // Build comma-separated lengths list
    std::string lengthsStr;
    for (size_t i = 0; i < s.usedLengths.size(); ++i) {
        lengthsStr += std::to_string(static_cast<int>(s.usedLengths[i])) + "ft";
        if (i + 1 < s.usedLengths.size()) lengthsStr += ", ";
    }

    std::cout << std::left << std::setw(10) << s.sectionName << " | "
              << std::setw(30) << lengthsStr << " | "
              << std::right << std::setw(7) << std::fixed << std::setprecision(1) << s.totalLength << " | "
              << "Rs. " << std::setw(4) << std::left << static_cast<int>(rate) << " | "
              << "Rs. " << static_cast<int>(totalPrice) << "\n";
    }


    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Grand Total Aluminum Cost = Rs. " << static_cast<int>(grand) << "\n";
}
double ManualRatesManager::getTotalAluminiumCost() const {
    return grand;  // ye final bill return karega
}

// ManualRatesManager.cpp me add karo
void ManualRatesManager::reset() {
    sectionRates.clear();   // saare purane rates hatao
    grand = 0.0;            // total reset

    // Har section ka price reset karo
    for (auto& s : sections) {
        s.totalPrice = 0.0;
    }

    std::cout << "🔄 ManualRatesManager data cleared!\n";
}
