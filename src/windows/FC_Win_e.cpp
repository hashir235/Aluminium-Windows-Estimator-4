#include "FC_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

int FC_Win_e::windowCount = 0; 
void FC_Win_e::resetWindowCount() { windowCount = 0;}
FC_Win_e::FC_Win_e()
    : height(0.0), width(0.0), rightWidth(0.0), leftWidth(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}

bool FC_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "FC_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!inputDetails()) return false;

    return true;
}

////////////////////////// Input Details Method //////////////////////////

bool FC_Win_e::inputDetails() {
    // 1. Unit choice
    while (true) {
        std::cout << "Enter size in (i)nches or (f)eet? [i/f] (0 = cancel): ";
        if (std::cin >> unitChoice) {
            if (unitChoice == '0') {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
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

    // 2. Collar type
    while (true) {
        std::cout << "Enter Collar Type (1 - 2, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 2) break;
        }
        std::cout << "❌ Please enter a number between 1 and 2.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 3. Height
    while (true) {
        std::cout << "Enter Height (0 = cancel): ";
        if (std::cin >> height) {
            if (height == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (height > 0) break;
        }
        std::cout << "❌ Height must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 4. Right Width
    while (true) {
        std::cout << "Enter Right Width (0 = cancel): ";
        if (std::cin >> rightWidth) {
            if (rightWidth == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (rightWidth > 0) break;
        }
        std::cout << "❌ Right Width must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 5. Left Width
    while (true) {
        std::cout << "Enter Left Width (0 = cancel): ";
        if (std::cin >> leftWidth) {
            if (leftWidth == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (leftWidth > 0) break;
        }
        std::cout << "❌ Left Width must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Convert inches to feet if needed
    if (inputInInches) {
        height     /= 12.0;
        rightWidth /= 12.0;
        leftWidth  /= 12.0;
    }

    // Set total width
    width = leftWidth + rightWidth;

    return true;
}


void FC_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double wr = rightWidth;
    double wl = leftWidth;

    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_D54F = settingsC.getMargin("D54F");
    double cm_D54A = settingsC.getMargin("D54A");
    double cm_D41 = settingsC.getMargin("D41");

    Section_Settings& settings = Section_Settings::getInstance();

    auto labelWith = [&](const std::string& prefix, double value) {
        return std::make_pair(label + prefix, value);
    };

    auto addSection = [&](const std::string& key, const std::vector<std::pair<std::string, double>>& values) {
        std::vector<std::pair<double, std::string>> converted;
        for (const auto& [lbl, val] : values) {
            converted.emplace_back(val, lbl);
        }
        sectionMap[settings.getSectionName(key)] = converted;
    };

    // Collar Type Sections
    if (collarType == 1) {
        addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F),
                               labelWith("WT_l", wl + cm_D54F), labelWith("WT_r", wr + cm_D54F),
                            labelWith("WB_l", wl + cm_D54F), labelWith("WB_r", wr + cm_D54F) });
    }
    else if (collarType == 2) {
        addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HL", h + cm_D54A),
                               labelWith("WT_l", wl + cm_D54A), labelWith("WT_r", wr + cm_D54A),
                            labelWith("WB_l", wl + cm_D54A), labelWith("WB_r", wr + cm_D54A) });
    }

    // Inner Sections - Corner Window
    addSection("D41", { labelWith("HL", h + cm_D41), labelWith("HR", h + cm_D41),
                               labelWith("WT_l", wl + cm_D41), labelWith("WT_r", wr + cm_D41),
                            labelWith("WB_l", wl + cm_D41), labelWith("WB_r", wr + cm_D41) });
}

double FC_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string FC_Win_e::getLabel() const {
    return label;
}

const SectionMap& FC_Win_e::getSectionMap() const {
    return sectionMap;
}


void FC_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char FC_Win_e::getUnit() const {return unitChoice;}   
double FC_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double FC_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
double FC_Win_e::getRightWidth() const { if (inputInInches) {return rightWidth * 12.0;} return rightWidth; }
double FC_Win_e::getLeftWidth() const { if (inputInInches) {return leftWidth * 12.0;} return leftWidth; }
int FC_Win_e::getCollarType() const { return collarType; }


void FC_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


