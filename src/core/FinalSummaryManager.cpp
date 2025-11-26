#include "FinalSummaryManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm> //  for std::remove_if


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

void FinalSummaryManager::setAluColor(const std::string& color) {
    AluColor = color;
}
void FinalSummaryManager::setContractor(const std::string& constractor){
    contractorName = constractor;
}

void FinalSummaryManager::setContect(const std::string& contect){
    contractorphoneNumber = contect;
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
        std::cout << "------------------------------------------------\n";
        std::cout << "Section   | Qty |   Area (sq.ft)\n";
        std::cout << "------------------------------------------------\n";
        for (const auto& [type, data] : windowTypeSummary) {
            std::cout << " " << std::setw(8) << std::left << type
                      << " | " << std::setw(3) << data.first
                      << " | " << std::fixed << std::setprecision(2) << data.second
                      << "\n";
        }
    }
    }

int FinalSummaryManager::displayFinalSummary() const {
    std::cout << "\n FINAL COST SUMMARY:\n";
    double totalGlassCost = 0.0;
    double totalLaborCost = 0.0;
    double totalHardwareCost = 0.0;

    int totalWindows = 0;
    double totalArea = 0.0;

    for (const auto& [type, data] : windowTypeSummary) {
        totalWindows += data.first;
        totalArea += data.second;
    }

    totalGlassCost = totalArea * glassRatePerSqFt;
    totalLaborCost = totalArea * laborRatePerSqFt;
    totalHardwareCost = static_cast<double>(totalWindows) * hardwareRatePerWindow;

    double discount = (aluminiumDiscountPercent / 100.0) * aluminiumTotalCost;
    double discountedAluminium = aluminiumTotalCost - discount;
    double netTotal = totalGlassCost + totalLaborCost + totalHardwareCost + discountedAluminium;

    std::cout << "=====================================================\n";
    std::cout << "=                Final Summary Result               =\n";
    std::cout << "=====================================================\n";

    if (!projectName.empty())  std::cout << " Project        : " << projectName << "\n";
    if (!location.empty())     std::cout << " Location       : " << location << "\n";
    if (!phoneNumber.empty())  std::cout << " Customer Ph.no : " << phoneNumber << "\n";
    if (!gageNumber.empty())   std::cout << " Gage Number    : " << gageNumber << "\n"; 
    if (!glassColor.empty())   std::cout << " Glass Color    : " << glassColor << "\n"; 
    if (!AluColor.empty())     std::cout << " Aluminium Color: " << AluColor << "\n"; 
    if (!contractorName.empty()) std::cout << " Workshop/Contractor Name: " << contractorName << "\n";
    if (!contractorphoneNumber.empty()) std::cout << " Contect no     : " << contractorphoneNumber << "\n";

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

    std::cout << "Add Other Charges (optional): PKR ";
    std::string commissionInput;
    std::getline(std::cin, commissionInput);
    if (!commissionInput.empty()) {
        try {
            double commissionValue = std::stod(commissionInput);
            if (commissionValue > 0) netTotal += commissionValue;
    std::cout << "Other Charges Added      = PKR " << commissionValue << "\n";
        } catch (...) {
            std::cout << " Invalid commission ignored.\n";
        }
    }

    std::cout << "_____________________________________________________\n";
    std::cout << "FINAL GRAND TOTAL         = PKR [" << netTotal << "]\n";
    std::cout << "_____________________________________________________\n";

// =======================
//  STEP 1: Menu choices
// =======================
int navChoice = 0;
while (true) {
    std::cout << "\n What do you want to do next?\n"
              << "   [1] Make a NEW bill (reset data)\n"
              << "   [2] Re-enter Other Rates\n"
              << "   [3] Download invoice                   (PDF)\n"
              << "   [4] Download Material Table            (PDF)\n"
              << "   [5] Download Length Optimization Table (PDF)\n"
              << "   [0] Go back to Home Menu\n"
              << "Enter choice: ";

    std::string input;
    if (!std::getline(std::cin >> std::ws, input)) return 0;
    try {
        navChoice = std::stoi(input);
        if (navChoice == 0 || navChoice == 1 ||
            navChoice == 2 || navChoice == 3 ||
            navChoice == 4 || navChoice == 5 ) break;
        std::cout << " Invalid choice! Please enter 0–4.\n";
    } catch (...) {
        std::cout << " Invalid input! Please enter a number.\n";
    }
}

// =========================
//  STEP 2: Handle choice
// =========================
if (navChoice == 3) {
    int odd, glh;

    std::cout << "Do you want to include ODD and GLH in the invoice? (1=Yes, 0=No): ";
    while (true) {
        if (std::cin >> odd && (odd == 0 || odd == 1)) break;
        std::cout << " Invalid input! Enter 1 or 0: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Do you want to include GLH in the invoice? (1=Yes, 0=No): ";
    while (true) {
        if (std::cin >> glh && (glh == 0 || glh == 1)) break;
        std::cout << " Invalid input! Enter 1 or 0: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Generating TXT and PDF, please wait...\n";

    // ---------------------------------------------------------
    // STEP 1: OVERWRITE EXISTING FILE (NO CREATION)
    // ---------------------------------------------------------
    std::ofstream file("data/Temp_invoice.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot write to data/Temp_invoice.txt\n";
        return navChoice;
    }

    file << "=====================================================\n";
    file << "=                Final Summary Result               =\n";
    file << "=====================================================\n";
    if (!projectName.empty())  file << " Project        : " << projectName << "\n";
    if (!location.empty())     file << " Location       : " << location << "\n";
    if (!phoneNumber.empty())  file << " Customer Ph.no : " << phoneNumber << "\n";
    if (!gageNumber.empty())   file << " Gage Number    : " << gageNumber << "\n";
    if (!glassColor.empty())   file << " Glass Color    : " << glassColor << "\n";
    if (!AluColor.empty())     file << " Aluminium Color: " << AluColor << "\n";
    if (!contractorName.empty()) file << " Workshop/Contractor: " << contractorName << "\n";
    if (!contractorphoneNumber.empty()) file << " Contect no         : " << contractorphoneNumber << "\n";
    file << "-----------------------------------------------------\n";
    file << "Win_type   | Qty |   Area (sq.ft)\n";
    file << "-----------------------------------------------------\n";
    for (const auto& [type, data] : windowTypeSummary) {
        file << " " << std::setw(8) << std::left << type
             << " | " << std::setw(3) << data.first
             << " | sq " << std::fixed << std::setprecision(2) << data.second
             << "\n";
    }
    file << "-----------------------------------------------------\n";
    file << "Total Windows/Doors/Arch = No:" << totalWindows << "\n";
    file << "Total Area (sq.ft)       = sq " << totalArea << "\n";
    file << "Net Rate (per sq.ft)     = Rs " << (totalArea > 0 ? netTotal / totalArea : 0.0) << "\n";
    if(odd == 1){
    file << "-----------------------------------------------------\n";
    file << "Aluminium Original       = Rs " << aluminiumTotalCost << "\n";
    file << "Aluminium Discount (" << aluminiumDiscountPercent << "%) = - Rs " << discount << "\n";
    file << "Aluminium After Discount = + Rs " << discountedAluminium << "\n";
    }
    if(glh == 1){
    file << "-----------------------------------------------------\n";
    file << "Glass Cost               = + Rs " << totalGlassCost << "\n";
    file << "Labor Cost               = + Rs " << totalLaborCost << "\n";
    file << "Hardware Cost            = + Rs " << totalHardwareCost << "\n";
    if (!commissionInput.empty()) {
        try {
            double commissionValue = std::stod(commissionInput);
            if (commissionValue > 0) {
                file << "Other Charges Added      = + Rs " << commissionValue << "\n";
                file << "--------------------------------------------------------\n";
            }
        } catch (...) {}
    }
   }
    file << "FINAL GRAND TOTAL         = PKR " << netTotal << "\n";
    file.close();

    // ---------------------------------------------------------
    // STEP 2: GENERATE PDF FILE NAME
    // ---------------------------------------------------------
    std::string baseName;

    if (!projectName.empty()) baseName = projectName;
    else if (!location.empty()) baseName = location;
    else if (!phoneNumber.empty()) baseName = phoneNumber;
    else baseName = "invoice";

    baseName.erase(std::remove_if(baseName.begin(), baseName.end(),
        [](char c) { return !std::isalnum(static_cast<unsigned char>(c)); }), baseName.end());

    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    std::ostringstream filename;
    filename << baseName << "_"
             << (1900 + localTime->tm_year)
             << (1 + localTime->tm_mon)
             << localTime->tm_mday << "_"
             << localTime->tm_hour
             << localTime->tm_min
             << localTime->tm_sec
             << ".pdf";

    std::string pdfName = filename.str();

    // ---------------------------------------------------------
    // STEP 3: PYTHON SCRIPT
    // ---------------------------------------------------------
    std::string outputPDFPath = "../Downloads/Invoice/" + pdfName;

    std::string command =
    "python scripts/make_invoice_pdf.py data/Temp_invoice.txt \"" + outputPDFPath + "\"";

    system(command.c_str());

    std::cout << " Invoice PDF created: " << outputPDFPath << "\n";
}

        return navChoice;
}

void FinalSummaryManager::removeWindowSummary(const std::string& label, double areaSqFt) {
    std::string typeKey = label.substr(0, label.find(' '));

    auto it = windowTypeSummary.find(typeKey);
    if (it != windowTypeSummary.end()) {
        it->second.first -= 1;         // quantity reduce karo
        it->second.second -= areaSqFt; // area reduce karo

        if (it->second.first <= 0 || it->second.second <= 0) {
            windowTypeSummary.erase(it); // agar 0 ho gaya to delete entry
        }
    }
}




