#include "FC_Win_f.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "fabricator_settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

int FC_Win_f::windowCount = 0; 
void FC_Win_f::resetWindowCount() { windowCount = 0;}
FC_Win_f::FC_Win_f()
    : height(0.0), width(0.0), rightWidth(0.0), leftWidth(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}

bool FC_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "FC_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!inputDetails()) return false;

    return true;
}

////////////////////////// Input Details Method //////////////////////////

bool FC_Win_f::inputDetails() {
   

    // 1. Collar type
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

    // 2. Height
    while (true) {
        std::cout << "Enter Height in Cm (0 = cancel): ";
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
        std::cout << "Enter Right Width in Cm (0 = cancel): ";
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

    // 4. Left Width
    while (true) {
        std::cout << "Enter Left Width in Cm (0 = cancel): ";
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

    return true;
}


void FC_Win_f::calculateSections() {
    sectionMap.clear();

    double h = height;
    double wr = rightWidth;
    double wl = leftWidth;
    double feet = 30.48; // cm to feet

     // 🔹 Fabricator Cutting Margin from fabricator_settings
    fabricator_settings& settingsD = fabricator_settings::getInstance();
    double cm = settingsD.getCuttingMargin();   // default 1.2 agar file me na mile

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
        addSection("D54F", { labelWith("HL", ((h + 6)+ cm)/ feet), labelWith("HR", ((h + 6)+ cm)/ feet),
                             labelWith("WT_l", ((wl + 6)+ cm)/ feet), labelWith("WT_r", ((wr + 6)+ cm)/ feet),
                             labelWith("WB_l", ((wl + 6)+ cm)/ feet), labelWith("WB_r",((wr + 6)+ cm)/ feet) });
    }
    else if (collarType == 2) {
        addSection("D54A", { labelWith("HL", ((h)+ cm)/ feet), labelWith("HR", ((h)+ cm)/ feet),
                             labelWith("WT_l", ((wl)+ cm)/ feet), labelWith("WT_r", ((wr)+ cm)/ feet),
                             labelWith("WB_l", ((wl)+ cm)/ feet), labelWith("WB_r", ((wr)+ cm)/ feet) });
    }
        // Inner Sections - Corner Window
        addSection("D41", { labelWith("HL", ((h - 3.4)+ cm)/ feet), labelWith("HR", ((h - 3.4)+ cm)/ feet),
                            labelWith("WT_l", ((wl - 1.7)+ cm)/ feet), labelWith("WT_r", ((wr - 1.7)+ cm)/ feet),
                            labelWith("WB_l", ((wl - 1.7)+ cm)/ feet), labelWith("WB_r", ((wr - 1.7)+ cm)/ feet) });
}

double FC_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}

std::string FC_Win_f::getLabel() const {
    return label;
}

const SectionMap& FC_Win_f::getSectionMap() const {
    return sectionMap;
}


void FC_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}
 
double FC_Win_f::getHeight() const { return height;}
double FC_Win_f::getWidth() const { return rightWidth + leftWidth; }
double FC_Win_f::getRightWidth() const { return rightWidth; }
double FC_Win_f::getLeftWidth() const { return leftWidth; }
int FC_Win_f::getCollarType() const { return collarType; }


void FC_Win_f::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


