#include "FinalSummaryManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>

void FinalSummaryManager::setGlassRate(double rate) {
    glassRatePerSqFt = rate;
}

void FinalSummaryManager::setLaborRate(double rate) {
    laborRatePerSqFt = rate;
}

void FinalSummaryManager::setHardwareRate(double rate) {
    hardwareRatePerWindow = rate;
}

void FinalSummaryManager::setAluminiumDiscount(double percentage) {
    aluminiumDiscountPercent = percentage;
}

void FinalSummaryManager::setAluminiumTotal(double total) {
    aluminiumTotalCost = total;
}

void FinalSummaryManager::setProjectName(const std::string& name) {
    projectName = name;
}
void FinalSummaryManager::setLocation(const std::string& loc) {
    location = loc;
}
void FinalSummaryManager::setPhoneNumber(const std::string& phone) {
    phoneNumber = phone;
}

void FinalSummaryManager::setGlassColor(const std::string& color) {
    glassColor = color;
}

void FinalSummaryManager::setGageNumber(const std::string& gage) {
    gageNumber = gage;
}

std::string FinalSummaryManager::getGageNumber() const {
    return gageNumber;
}

void FinalSummaryManager::addWindow(const std::string& label, double areaSqFt) {
    // Extract type key like "S_Win"
    std::string typeKey = label.substr(0, label.find(' '));

    // Agar pehle se exist karta hai to qty++ aur area add karo
    if (windowTypeSummary.find(typeKey) != windowTypeSummary.end()) {
        windowTypeSummary[typeKey].first += 1;
        windowTypeSummary[typeKey].second += areaSqFt;
    } else {
        // Naya entry banao
        windowTypeSummary[typeKey] = {1, areaSqFt};
    }
}


void FinalSummaryManager::reset() {
    glassRatePerSqFt = 0;
    laborRatePerSqFt = 0;
    hardwareRatePerWindow = 0;
    aluminiumDiscountPercent = 0;
    aluminiumTotalCost = 0;
    projectName.clear();
    location.clear();
    phoneNumber.clear();
    gageNumber.clear();
}

void FinalSummaryManager::print_qty_sq() const {
    if (!windowTypeSummary.empty()) {
        std::cout << "-------------------------------------\n";
        std::cout << "Section   | Qty |   Area (sq.ft)\n";
        std::cout << "-------------------------------------\n";
        for (const auto& [type, data] : windowTypeSummary) {
            std::cout << " " << std::setw(8) << std::left << type
                      << " | " << std::setw(3) << data.first
                      << " | " << std::fixed << std::setprecision(2) << data.second
                      << "\n";
        }
        std::cout << "-------------------------------------\n";
    }
    }

int FinalSummaryManager::displayFinalSummary() const {
    std::cout << "\n📊 FINAL COST SUMMARY:\n";
    double totalGlassCost = 0.0;
    double totalLaborCost = 0.0;
    double totalHardwareCost = 0.0;

    int totalWindows = 0;
    double totalArea = 0.0;

    for (const auto& [type, data] : windowTypeSummary) {
        totalWindows += data.first;   // qty
        totalArea += data.second;     // total area
    }

    totalGlassCost = totalArea * glassRatePerSqFt;
    totalLaborCost = totalArea * laborRatePerSqFt;
    totalHardwareCost = static_cast<double>(totalWindows) * hardwareRatePerWindow;

    double discount = (aluminiumDiscountPercent / 100.0) * aluminiumTotalCost;
    double discountedAluminium = aluminiumTotalCost - discount;
    double netTotal = totalGlassCost + totalLaborCost + totalHardwareCost + discountedAluminium;
    double commission = 0.0;   // ✅ bas ek hi dafa declare karo

    std::cout << "=====================================================\n";
    std::cout << "=                Final Summary Result               =\n";
    std::cout << "=====================================================\n";

    if (!projectName.empty())  std::cout << " Project     : " << projectName << "\n";
    if (!location.empty())     std::cout << " Location   : " << location << "\n";
    if (!phoneNumber.empty())  std::cout << " Phone      : " << phoneNumber << "\n";
    if (!gageNumber.empty())   std::cout << " Gage Number: " << gageNumber << " mm\n"; 
    if (!glassColor.empty())   std::cout << " Glass Color: " << glassColor << " \n"; 

    print_qty_sq();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Total Windows/Doors      =     " << totalWindows << "\n";
    std::cout << "Total Area (sq.ft)       =     " << totalArea << "\n";
    std::cout << "Net Rate (per sq.ft)     = PKR " << (totalArea > 0 ? netTotal / totalArea : 0.0) << "\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Glass Cost               = PKR " << totalGlassCost << "\n";
    std::cout << "Labor Cost               = PKR " << totalLaborCost << "\n";
    std::cout << "Hardware Cost            = PKR " << totalHardwareCost << "\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Aluminium Original       = PKR " << aluminiumTotalCost << "\n";
    std::cout << "Aluminium Discount (" << aluminiumDiscountPercent << "%) = PKR " << discount << "\n";
    std::cout << "Aluminium After Discount = PKR " << discountedAluminium << "\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "GRAND TOTAL              = PKR " << netTotal << "\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Add Commission (optional): PKR ";
    std::string commissionInput;
    std::getline(std::cin, commissionInput);

    double commissionValue = 0.0;
    if (!commissionInput.empty()) {
    try {
        commissionValue = std::stod(commissionInput);
        if (commissionValue > 0) {
            netTotal += commissionValue;
        }
    } catch (...) {
        std::cout << "⚠️ Invalid commission ignored.\n";
    }
    }

   std::cout << "_____________________________________________________\n";
   std::cout << "FINAL GRAND TOTAL         = PKR [" << netTotal << "]\n";
   std::cout << "_____________________________________________________\n";

  // --- Safe menu choice input ---
int navChoice = 0;
while (true) {
    std::cout << "\n➡ What do you want to do next?\n"
              << "   [1] Make a NEW bill (reset data)\n"
              << "   [2] Re-enter Other Rates\n"
              << "   [0] Go back to Home Menu\n"
              << "Enter choice: ";

    std::string input;
    if (!std::getline(std::cin >> std::ws, input)) {
        // EOF or stream error — safe default to Home
        return 0;  
    }

    try {
        navChoice = std::stoi(input);
        if (navChoice == 0 || navChoice == 1 || navChoice == 2) {
            break;
        }
        std::cout << "⚠️ Invalid choice! Please enter 0, 1, or 2.\n";
    } catch (...) {
        std::cout << "⚠️ Invalid input! Please enter a number (0, 1, or 2).\n";
    }
}


    return navChoice;
}

void FinalSummaryManager::removeWindowSummary(const std::string& label, double areaSqFt) {
    std::string typeKey = label.substr(0, label.find(' '));

    auto it = windowTypeSummary.find(typeKey);
    if (it != windowTypeSummary.end()) {
        it->second.first -= 1;        // qty reduce
        it->second.second -= areaSqFt; // area reduce

        if (it->second.first <= 0 || it->second.second <= 0) {
            windowTypeSummary.erase(it); // agar sab khatam ho gya to delete entry
        }
    }
}


