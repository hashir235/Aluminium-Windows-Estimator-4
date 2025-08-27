#include "FinalCostCalculator.h"
#include "FinalSummaryManager.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm> // for all_of

extern FinalSummaryManager summaryManager;  // Connected globally from main

FinalCostCalculator::FinalCostCalculator() {
    // No special initialization needed for now
}

void FinalCostCalculator::inputRates() {
    double glassRate = 0, laborRate = 0, hardwareRate = 0, discount = 0;
    
   auto getValidInput = [](const std::string& prompt) ->double {
   double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid input! Please enter a number.\n";
        }
        else if (value < 0) {
            std::cout << "⚠️ Value must be 0 or greater! Try again.\n";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value; // ✅ 0 is allowed
        }
    }
};


    auto getDiscountInput = [](const std::string& prompt) ->double {
       double value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "❌ Invalid input! Please enter a number.\n";
            }
            else if (value < 0 || value > 100) {
                std::cout << "⚠️ Discount must be between 0 and 100%! Try again.\n";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
    };

    auto getOptionalString = [](const std::string& prompt) -> std::string {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    };

    auto getPhoneNumber = [](const std::string& prompt) -> std::string {
        std::string value;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, value);

            bool isValid = !value.empty() && value.size() <= 15 &&
                           std::all_of(value.begin(), value.end(), ::isdigit);

            if (isValid || value.empty()) {
                return value; // allow empty
            } else {
                std::cout << "⚠️ Phone must be digits only and max 15 characters. Try again.\n";
            }
        }
    };

    // Main confirmation loop
    while (true) {
        // Step 1: Take all inputs
        glassRate    = getValidInput("\n🧮 Enter Glass Rate per sq.ft: Rs ");
        laborRate    = getValidInput("🔧 Enter Labor Rate per sq.ft: Rs ");
        hardwareRate = getValidInput("🔩 Enter Hardware Rate per window: Rs ");
        discount     = getDiscountInput("💸 Enter Aluminium Discount Percentage: % ");
        

        // Step 3: Show summary for confirmation
       std::string projectName, location, phoneNumber;
       std::string gageNumber; // sirf naya variable


        // Step 2: Optional Project Details
        std::cout << "\n📌 (Optional) Enter project details:\n";
        projectName = getOptionalString(" Project Name: ");
        location    = getOptionalString(" Location: ");
        phoneNumber = getPhoneNumber(" Phone Number (max 15 digits): ");
        gageNumber  = getOptionalString(" Gage Number (mm): "); // New optional field
        glassColor  = getOptionalString(" Glass Color: "); // New optional field

        // Step 3: Show summary for confirmation
        std::cout << "\n📋 Please confirm the entered details:\n";
        std::cout << "Glass Rate:      Rs " << glassRate << " per sq.ft\n";
        std::cout << "Labor Rate:      Rs " << laborRate << " per sq.ft\n";
        std::cout << "Hardware Rate:   Rs " << hardwareRate << " per window\n";
        std::cout << "Aluminium Discount: " << discount << "%\n";
        if (!projectName.empty()) std::cout << " Project: " << projectName << "\n";
        if (!location.empty())    std::cout << " Location: " << location << "\n";
        if (!phoneNumber.empty()) std::cout << " Phone: " << phoneNumber << "\n";
        if (!gageNumber.empty())  std::cout << " Gage Number: " << gageNumber << " mm\n"; // Print if entered
        if (!gageNumber.empty())  std::cout << " Glass Color: " << glassColor << " mm\n"; // Print if entered

        // Step 4: Confirmation
        std::cout << "\n✔ Confirm all values? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            // Save all values to summary manager
            summaryManager.setGlassRate(glassRate);
            summaryManager.setLaborRate(laborRate);
            summaryManager.setHardwareRate(hardwareRate);
            summaryManager.setAluminiumDiscount(discount);
            summaryManager.setProjectName(projectName);
            summaryManager.setLocation(location);
            summaryManager.setPhoneNumber(phoneNumber);
            summaryManager.setGageNumber(gageNumber); 
            summaryManager.setGlassColor(glassColor); 
            break;
        }
        else {
            std::cout << "\n🔄 Let's re-enter all the details again.\n";
        }
    }
}

int FinalCostCalculator::displayFinalSummary() {
   return summaryManager.displayFinalSummary();
}
