#include "F_Win_f.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "fabricator_settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int F_Win_f::windowCount = 0; 
void F_Win_f::resetWindowCount() { windowCount = 0;}
F_Win_f::F_Win_f() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

bool F_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {
        label = "F_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }


    // 🔹 1. Collar Type
    while (true) {
        std::cout << "Enter Collar Type (1 - 14, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 14)
                break;
        }
        std::cout << "❌ Please enter a number between 1 and 14.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 🔹 2. Height (in cm)
    while (true) {
        std::cout << "Enter Height (in cm, 0 = cancel): ";
        if (std::cin >> height) {
            if (height == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (height > 0)
                break;
        }
        std::cout << "❌ Height must be a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 🔹 3. Width (in cm)
    while (true) {
        std::cout << "Enter Width (in cm, 0 = cancel): ";
        if (std::cin >> width) {
            if (width == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (width > 0)
                break;
        }
        std::cout << "❌ Width must be a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return true;
}




void F_Win_f::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;
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
        for (const auto& [label, val] : values) {
            converted.emplace_back(val, label);
        }
        sectionMap[settings.getSectionName(key)] = converted;
    };

switch (collarType) {
        case 1:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("HR", ((h + 6)+ cm )/ feet), labelWith("WT", ((w + 6)+ cm )/ feet), labelWith("WB", ((w + 6)+ cm )/ feet) });
            break;
        case 2:
            addSection("D54A", { labelWith("HL",((h)+ cm )/ feet), labelWith("HR",((h)+ cm )/ feet), labelWith("WT",((w)+ cm )/ feet), labelWith("WB", ((w)+ cm )/ feet) });
            break;
        case 3:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("HR", ((h + 6)+ cm )/ feet), labelWith("WB", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("WT", ((w)+ cm )/ feet) });
            break;
        case 4:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("WT", ((w + 6)+ cm )/ feet), labelWith("WB", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HR", ((h)+ cm )/ feet) });
            break;
        case 5:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("HR", ((h + 6)+ cm )/ feet), labelWith("WT", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("WB", ((w)+ cm )/ feet) });
            break;
        case 6:
            addSection("D54F", { labelWith("HR", ((h + 6)+ cm )/ feet), labelWith("WT", ((w + 6)+ cm )/ feet), labelWith("WB", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet) });
            break;
        case 7:
            addSection("D54F", { labelWith("HR", ((h + 6)+ cm )/ feet), labelWith("WB",((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet), labelWith("WT", ((w)+ cm )/ feet) });
            break;
        case 8:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("WT", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HR", ((h)+ cm )/ feet), labelWith("WB", ((w)+ cm )/ feet) });
            break;
        case 9:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet), labelWith("HR", ((h + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("WB", ((w)+ cm )/ feet), labelWith("WT",((w)+ cm )/ feet) });
            break;
        case 10:           
            addSection("D54F", { labelWith("WT", ((w + 6)+ cm )/ feet), labelWith("WB", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet), labelWith("HR", ((h)+ cm )/ feet) });
            break;
        case 11:
            addSection("D54F", { labelWith("WT", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet), labelWith("HR", ((h)+ cm )/ feet), labelWith("WB", ((w)+ cm )/ feet) });
            break;
        case 12:
            addSection("D54F", { labelWith("HR", ((h + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet), labelWith("WT", ((w)+ cm )/ feet), labelWith("WB", ((w)+ cm )/ feet) });
            break;
        case 13:
            addSection("D54F", { labelWith("WB", ((w + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HL", ((h)+ cm )/ feet), labelWith("HR", ((h)+ cm )/ feet), labelWith("WT", ((w)+ cm )/ feet) });     
            break;
        case 14:
            addSection("D54F", { labelWith("HL", ((h + 6)+ cm )/ feet) });
            addSection("D54A", { labelWith("HR", ((h)+ cm )/ feet), labelWith("WT", ((w)+ cm )/ feet), labelWith("WB", ((w)+ cm )/ feet) });        
            break;
    }

    // Inner Sections
    addSection("D41", { labelWith("HL", ((h - 3.4)+ cm )/ feet), labelWith("HR",((h - 3.4)+ cm )/ feet ), labelWith("WT", ((h - 3.4)+ cm )/ feet ), labelWith("WB", ((h - 3.4)+ cm )/ feet ) });
}

double F_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}

std::string F_Win_f::getLabel() const {
    return label;
}

const SectionMap& F_Win_f::getSectionMap() const {
    return sectionMap;
}


void F_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char F_Win_f::getSize() const {return unitChoice;}   
double F_Win_f::getHeight() const {  return height;}
double F_Win_f::getWidth() const { return width;}
int F_Win_f::getCollarType() const { return collarType; }


void F_Win_f::sendToEstimator() {
    std::cout << "[F_Win_f] Warning: No estimator provided in sendToEstimator().\n";
}
