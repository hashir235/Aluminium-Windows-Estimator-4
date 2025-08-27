#include "SM_Win.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include <iostream>
#include <limits>
#include <string>


void SM_Win::resetWindowCount() { windowCount = 0;} 
int SM_Win::windowCount = 0;
SM_Win::SM_Win() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

bool SM_Win::inputWindowData(bool isEditing ) {
    if (!isEditing) {
        
        label = "SM_Win #" + std::to_string(windowCount) + " -> ";
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


void SM_Win::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;

    SettingsManagerC& settingsC = SettingsManagerC::getInstance();
    double cm_M30 = settingsC.getMargin("M30");
    double cm_M26F = settingsC.getMargin("M26F");
    double cm_M26 = settingsC.getMargin("M26");
    double cm_M30F = settingsC.getMargin("M30F");
    double cm_M23 = settingsC.getMargin("M23");
    double cm_M28 = settingsC.getMargin("M28");
    double cm_M24 = settingsC.getMargin("M24");
    double cm_D29 = settingsC.getMargin("D29");
    

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
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("HR", h + cm_M30F), labelWith("WT", w + cm_M30F) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 2:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("HR", h + cm_M30), labelWith("WT", w + cm_M30) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 3:
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("HR", h + cm_M30F) });
            addSection("M30", { labelWith("WT", w + cm_M30) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 4:
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("WT", w + cm_M30F) });
            addSection("M30", { labelWith("HR", h + cm_M30) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) }); 
            break;
        case 5:
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("HR", h + cm_M30F), labelWith("WT", w + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 6:
            addSection("M30", { labelWith("HL", h + cm_M30) });
            addSection("M30F", { labelWith("HR", h + cm_M30F), labelWith("WT", w + cm_M30F) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 7:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("WT", w + cm_M30) });
            addSection("M30F", { labelWith("HR", h + cm_M30F) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 8:
            addSection("M30", { labelWith("HR", h + cm_M30) });
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("WT", w + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 9:
            addSection("M30", { labelWith("WT", w + cm_M30) });
            addSection("M30F", { labelWith("HL", h + cm_M30F), labelWith("HR", h + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 10:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("HR", h + cm_M30) });
            addSection("M30F", { labelWith("WT", w + cm_M30F) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 11:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("HR", h + cm_M30) });
            addSection("M30F", { labelWith("WT", w + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 12:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("WT", w + cm_M30) });
            addSection("M30F", { labelWith("HR", h + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
        case 13:
            addSection("M30", { labelWith("HL", h + cm_M30), labelWith("HR", h + cm_M30), labelWith("WT", w + cm_M30) });
            addSection("M26F", { labelWith("WB", w + cm_M26F) });
            break;
        case 14:
            addSection("M30", { labelWith("HR", h + cm_M30), labelWith("WT", w + cm_M30) });
            addSection("M30F", { labelWith("HL", h + cm_M30F) });
            addSection("M26", { labelWith("WB", w + cm_M26) });
            break;
    }

    // Inner Sections
    addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
    addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
    addSection("M24", { labelWith("W1", (w / 2) + cm_M24), labelWith("W2", (w / 2) + cm_M24),
                        labelWith("W3", (w / 2) + cm_M24), labelWith("W4", (w / 2) + cm_M24)
                      });
    // Note! D29 is not included
}

double SM_Win::getAreaSqFt() const {
    return height * width;
}

std::string SM_Win::getLabel() const {
    return label;
}

const SectionMap& SM_Win::getSectionMap() const {
    return sectionMap;
}

void SM_Win::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char SM_Win::getSize() const {return unitChoice;}
double SM_Win::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double SM_Win::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int SM_Win::getCollarType() const { return collarType; }

void SM_Win::sendToEstimator() {
    std::cout << "[SM_Win] Warning: No estimator provided.\n";
}
