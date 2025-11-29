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

/////////////////////////////////////// 

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

void FinalSummaryManager::displayFinalSummary() const {
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

    // -----------------------------
    // BASE NET TOTAL
    // -----------------------------
    double netTotal = totalGlassCost + totalLaborCost + totalHardwareCost + discountedAluminium;

    // ============================================
    //   LOOP to add other charges (optional)
    // ============================================
    while (true)
    {
        // ======= Print Full Final Summary =======
        std::cout << "\n FINAL COST SUMMARY:\n";
        std::cout << "=====================================================\n";
        std::cout << "=                Final Summary Result               =\n";
        std::cout << "=====================================================\n";

        if (!projectName.empty())  std::cout << " Customer       : " << projectName << "\n";
        if (!location.empty())     std::cout << " Location       : " << location << "\n";
        if (!phoneNumber.empty())  std::cout << " Customer Ph.no : " << phoneNumber << "\n";
        if (!gageNumber.empty())   std::cout << " Gage Number    : " << gageNumber << "\n";
        if (!glassColor.empty())   std::cout << " Glass Color    : " << glassColor << "\n";
        if (!AluColor.empty())     std::cout << " Aluminium Color: " << AluColor << "\n";
        if (!contractorName.empty()) std::cout << " Contractor/Workshop  : " << contractorName << "\n";
        if (!contractorphoneNumber.empty()) std::cout << " Contact no     : " << contractorphoneNumber << "\n";

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
        if(!(extra == 0.0))
        std::cout << "Other Charges            = PKR " << extra << "\n";
        std::cout << "-----------------------------------------------------\n";
        std::cout << "GRAND TOTAL              = PKR " << netTotal << "\n";
        std::cout << "-----------------------------------------------------\n";

        // -------------------------------------------------------
        //   ASK: Any Other Charges?
        // -------------------------------------------------------
        std::cout << "\nAdd Other Charges (optional, press ENTER to skip): PKR ";

        std::string input;
        std::getline(std::cin >> std::ws, input);

        if (input.empty()) {
            // user skipped → final summary complete
            break;
        }

        try {
             extra = std::stod(input);
            if (extra > 0) {
                netTotal += extra;
                std::cout << "Added: PKR " << extra << "\n";
            } else {
                std::cout << " Invalid input ignored.\n";
            }
        }
        catch (...) {
            std::cout << " Invalid input ignored.\n";
        }

        // Continue loop → print summary again with updated total
    }  
}


/////////////////////////////////////////// generateInvoicePDF //////////////////////////////////////////

void FinalSummaryManager::generateInvoicePDF() const {

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

    int odd = 0, glh = 0;
    std::cout << "Include Aluminium details? (ODD) (1=Yes,0=No): ";
    std::cin >> odd;

    std::cout << "Include GLH (Glass/Labor/Hardware)? (1=Yes,0=No): ";
    std::cin >> glh;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::ofstream file("data/Temp_invoice.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot write to data/Temp_invoice.txt\n";
        return;
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

    if (odd == 1) {
        file << "-----------------------------------------------------\n";
        file << "Aluminium Original       = Rs " << aluminiumTotalCost << "\n";
        file << "Aluminium Discount (" << aluminiumDiscountPercent << "%) = - Rs " << discount << "\n";
        file << "Aluminium After Discount = + Rs " << discountedAluminium << "\n";
    }

    if (glh == 1) {
        file << "-----------------------------------------------------\n";
        file << "Glass Cost               = + Rs " << totalGlassCost << "\n";
        file << "Labor Cost               = + Rs " << totalLaborCost << "\n";
        file << "Hardware Cost            = + Rs " << totalHardwareCost << "\n";
    }
    if (!(extra == 0.0))
        file << "Other Charges            = + Rs " << extra << "\n";
    file << "-----------------------------------------------------\n";
    file << "FINAL GRAND TOTAL         = PKR " << netTotal << "\n";
    file << "-----------------------------------------------------\n";
    file.close();


    // ---------- Create Filename ----------
    std::string baseName;
    if (!projectName.empty()) baseName = projectName;
    else if (!location.empty()) baseName = location;
    else if (!phoneNumber.empty()) baseName = phoneNumber;
    else baseName = "invoice";

    baseName.erase(std::remove_if(
        baseName.begin(), baseName.end(),
        [](char c) { return !std::isalnum(static_cast<unsigned char>(c)); }
    ), baseName.end());

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
    std::string outputPDFPath = "../Downloads/Invoice/" + pdfName;

    std::string command =
        "python scripts/make_invoice_pdf.py data/Temp_invoice.txt \"" + outputPDFPath + "\"";

    int res = system(command.c_str());
    if (res == 0)
        std::cout << " Invoice PDF created: " << outputPDFPath << "\n";
    else
        std::cout << " PDF generation failed.\n";
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




