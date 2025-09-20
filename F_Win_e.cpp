#include "F_Win_e.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int F_Win_e::windowCount = 0; 
void F_Win_e::resetWindowCount() { windowCount = 0;}
F_Win_e::F_Win_e() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

// Returns true if data entered successfully, false if cancelled
bool F_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "F_Win_e #" + std::to_string(windowCount) + " -> ";
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

    if (inputInInches) {
        height /= 12.0;
        width  /= 12.0;
    }
    return true;
}



void F_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;

    SettingsManagerC& settingsC = SettingsManagerC::getInstance();
    double cm_D54F = settingsC.getMargin("D54F");
    double cm_D54A = settingsC.getMargin("D54A");
    double cm_D41 = settingsC.getMargin("D41");
  
    SettingsManagerA& settings = SettingsManagerA::getInstance();

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
    addSection("D41", { labelWith("HL", h + cm_D41), labelWith("HR", h + cm_D41 ), labelWith("WT", w + cm_D41 ), labelWith("WB", w + cm_D41 ) });
}

double F_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string F_Win_e::getLabel() const {
    return label;
}

const SectionMap& F_Win_e::getSectionMap() const {
    return sectionMap;
}


void F_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char F_Win_e::getSize() const {return unitChoice;}   
double F_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double F_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int F_Win_e::getCollarType() const { return collarType; }


void F_Win_e::sendToEstimator() {
    std::cout << "[F_Win_e] Warning: No estimator provided in sendToEstimator().\n";
}
