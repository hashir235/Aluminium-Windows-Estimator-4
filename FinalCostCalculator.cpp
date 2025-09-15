#include "FinalCostCalculator.h"
#include "FinalSummaryManager.h"
#include "AutoRatesManager.h"   // ✅ Auto mode ke liye include
#include <iostream>
#include <limits>
#include <string>
#include <algorithm> // for all_of

extern FinalSummaryManager summaryManager;   // Connected globally from main
extern AutoRatesManager autoRatesManager;    // ✅ Auto manager globally available

FinalCostCalculator::FinalCostCalculator() {
    // No special initialization needed for now
}

void FinalCostCalculator::inputRates() {
    double glassRate = 0, laborRate = 0, hardwareRate = 0, discount = 0;

    // ---------------- Validation Lambdas ----------------
    auto getValidInput = [](const std::string& prompt) -> double {
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

    auto getDiscountInput = [](const std::string& prompt) -> double {
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
            }
            else {
                std::cout << "⚠️ Phone must be digits only and max 15 characters. Try again.\n";
            }
        }
    };

    // ---------------- Main confirmation loop ----------------
    while (true) {
        // Step 1: Take all inputs
        glassRate = getValidInput("\n🧮 Enter Glass Rate per sq.ft: Rs ");
        laborRate = getValidInput("🔧 Enter Labor Rate per sq.ft: Rs ");
        hardwareRate = getValidInput("🔩 Enter Hardware Rate per window: Rs ");
        discount = getDiscountInput("💸 Enter Aluminium Discount Percentage: % ");

        // Step 2: Optional Project Details
        std::cout << "\n📌 (Optional) Enter project details:\n";
        projectName = getOptionalString(" Project Name: ");
        location = getOptionalString(" Location: ");
        phoneNumber = getPhoneNumber(" Phone Number (max 15 digits): ");

        // ✅ Auto vs Manual Mode Handling
        if (autoRatesManager.isAutoRatesEnabled()) {
            // ---------- Auto Mode ----------
            int thick = autoRatesManager.getThicknessChoice();
            if (thick == 1) gageNumber = "1.2mm";
            else if (thick == 2) gageNumber = "1.6mm";
            else if (thick == 3) gageNumber = "2.0mm";

            int color = autoRatesManager.getColorChoice();
            if (color == 1) AluColor = "DULL";
            else if (color == 2) AluColor = "H23/PC-RAL";
            else if (color == 3) AluColor = "SAHRA/BRN";
            else if (color == 4) AluColor = "BLACK/MULTI";
            else if (color == 5) AluColor = "WOOD COAT";

            // Glass color manual input (optional)
            glassColor = getOptionalString(" Glass Color: ");

        }
        else {
            // ---------- Manual Mode ----------
            gageNumber = getOptionalString(" Gage Number (mm): ");
            AluColor = getOptionalString(" Aluminium Color: ");
            glassColor = getOptionalString(" Glass Color: ");
        }

        // Step 3: Show summary for confirmation
        std::cout << "\n📋 Please confirm the entered details:\n";
        std::cout << "Glass Rate:      Rs " << glassRate << " per sq.ft\n";
        std::cout << "Labor Rate:      Rs " << laborRate << " per sq.ft\n";
        std::cout << "Hardware Rate:   Rs " << hardwareRate << " per window\n";
        std::cout << "Aluminium Discount: " << discount << "%\n";
        if (!projectName.empty()) std::cout << " Project: " << projectName << "\n";
        if (!location.empty()) std::cout << " Location: " << location << "\n";
        if (!phoneNumber.empty()) std::cout << " Phone: " << phoneNumber << "\n";
        if (!gageNumber.empty()) std::cout << " Gage Number: " << gageNumber << "\n";
        if (!AluColor.empty()) std::cout << " Aluminium Color: " << AluColor << "\n";
        if (!glassColor.empty()) std::cout << " Glass Color: " << glassColor << "\n";

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
            summaryManager.setAluColor(AluColor);
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
