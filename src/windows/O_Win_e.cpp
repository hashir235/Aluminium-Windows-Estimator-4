#include "O_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int O_Win_e::windowCount = 0; 
void O_Win_e::resetWindowCount() { windowCount = 0;}
O_Win_e::O_Win_e() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

// Returns true if data entered successfully, false if cancelled
bool O_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {

        label = "O_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    // Unit choice
    
    while (true) {
        std::cout << "Enter size in (i)nches or (f)eet? [i/f] (0 = cancel): ";
        if (std::cin >> unitChoice) {
            if (unitChoice == '0') {
                std::cout << "⚠ Entry cancelled!\n";
                return false; // cancelled
            }
            if (unitChoice == 'i' || unitChoice == 'I' || unitChoice == 'f' || unitChoice == 'F') {
                inputInInches = (unitChoice == 'i' || unitChoice == 'I');
                break;
            }
        }
        std::cout << "❌ Invalid input! Please enter 'i' or 'f'.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Collar type
    while (true) {
        std::cout << "Enter Collar Type (1 - 14, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 14) break;
        }
        std::cout << "❌ Please enter a number between 1 and 14.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Height
    while (true) {
        std::cout << "Enter Height (0 = cancel): ";
        if (std::cin >> height) {
            if (height == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (height > 0) break;
        }
        std::cout << "❌ Height must be a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Width
    while (true) {
        std::cout << "Enter Width (0 = cancel): ";
        if (std::cin >> width) {
            if (width == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (width > 0) break;
        }
        std::cout << "❌ Width must be a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    // Extra option: Add net (D29)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover input
    std::cout << "Add also net (D29) ? (y/n): ";
    std::getline(std::cin, netChoice);

    if (!netChoice.empty() && (netChoice == "y" || netChoice == "Y")) {
        std::cout << "✅ Net (D29) added!\n";
    }

    if (inputInInches) {
        height /= 12.0;
        width  /= 12.0;
    }

    return true;
}




void O_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;

    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_D54F = settingsC.getMargin("D54F");
    double cm_D54A = settingsC.getMargin("D54A");
    double cm_D50 = settingsC.getMargin("D50");
    double cm_D29   = settingsC.getMargin("D29");


    Section_Settings& settings = Section_Settings::getInstance();

    auto labelWith = [&](const std::string& prefix, double value) {
        return std::make_pair(label + prefix, value);
    };

    auto addSection = [&](const std::string& key, const std::vector<std::pair<std::string, double>>& values) {
        std::vector<std::pair<double, std::string>> converted;
        for (const auto& [label, val] : values) {
            converted.emplace_back(val, label);
        }
        sectionMap[settings.getSectionName(key)] = converted;
    };

    switch (collarType) {
        case 1:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F), labelWith("WT", w + cm_D54F), labelWith("WB", w + cm_D54F) });
            break;
        case 2:
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A), labelWith("WT", w + cm_D54A), labelWith("WB", w + cm_D54A) });
            break;
        case 3:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F), labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("WT", w + cm_D54A) });
            break;
        case 4:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("WT", w + cm_D54F), labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("HR", h + cm_D54A) });
            break;
        case 5:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F), labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("WB", w + cm_D54A) });
            break;
        case 6:
            addSection("D54F", { labelWith("HR", h + cm_D54F), labelWith("WT", w + cm_D54F), labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A) });
            break;
        case 7:
            addSection("D54F", { labelWith("HR", h + cm_D54F), labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("WT", w + cm_D54A) });
            break;
        case 8:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("HR", h + cm_D54A), labelWith("WB", w + cm_D54A) });
            break;
        case 9:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F) });
            addSection("D54A", { labelWith("WB", w + cm_D54A), labelWith("WT", w + cm_D54A) });
            break;
        case 10:           
            addSection("D54F", { labelWith("WT", w + cm_D54F), labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A) });
            break;
        case 11:
            addSection("D54F", { labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A), labelWith("WB", w + cm_D54A) });
            break;
        case 12:
            addSection("D54F", { labelWith("HR", h + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("WT", w + cm_D54A), labelWith("WB", w + cm_D54A) });
            break;
        case 13:
            addSection("D54F", { labelWith("WB", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A), labelWith("WT", w + cm_D54A) });     
            break;
        case 14:
            addSection("D54F", { labelWith("HL", h + cm_D54F) });
            addSection("D54A", { labelWith("HR", h + cm_D54A), labelWith("WT", w + cm_D54A), labelWith("WB", w + cm_D54A) });        
            break;
    }

        // Inner Sections
        addSection("D50A", { labelWith("HL", h + cm_D50), labelWith("HR", h + cm_D50 ), labelWith("WT", w + cm_D50 ), labelWith("WB", w + cm_D50 ) });

    if(netChoice == "y" || netChoice == "Y") {
        addSection("D29", { labelWith("HL", h + cm_D29), labelWith("HR", h + cm_D29), labelWith("WT", w + cm_D29), labelWith("WB", w + cm_D29) });
    }
}

double O_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string O_Win_e::getLabel() const {
    return label;
}

const SectionMap& O_Win_e::getSectionMap() const {
    return sectionMap;
}


void O_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char O_Win_e::getUnit() const { return unitChoice; }
double O_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double O_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int O_Win_e::getCollarType() const { return collarType; }


void O_Win_e::sendToEstimator() {
    std::cout << "[O_Win_e] Warning: No estimator provided in sendToEstimator().\n";
}
