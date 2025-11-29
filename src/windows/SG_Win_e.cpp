#include "SG_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

void SG_Win_e::resetWindowCount() { windowCount = 0;} 
int SG_Win_e::windowCount = 0; 
SG_Win_e::SG_Win_e() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

// Returns true if data entered successfully, false if cancelled
bool SG_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "SG_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectGlassOption()) return false;
    if (!inputSizeAndCollar()) return false;

    return true;
}

bool SG_Win_e::selectGlassOption() {
    while (true) {
        std::cout << "\n| |   | | Center Fix              (1) ";
        std::cout << "\n| | | | | Center Slide            (2) ";
        std::cout << "\n | | | |  Equal three glass part  (3) ";
        std::cout << "\n Select Option or 0 to cancel: ";

        if (std::cin >> glassOption) {
            if (glassOption == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (glassOption >= 1 && glassOption <= 3) break;
        }
        std::cout << "❌ Invalid input! Please enter 1, 2, or 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

bool SG_Win_e::inputSizeAndCollar() {
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
        std::cout << "❌ Height must be a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 4. Width
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

    // Convert inches to feet if needed
    if (inputInInches) {
        height /= 12.0;
        width  /= 12.0;
    }

    return true;
}


void SG_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;
    
    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_DC30C = settingsC.getMargin("DC30C");
    double cm_DC26F = settingsC.getMargin("DC26F");
    double cm_DC26C = settingsC.getMargin("DC26C");
    double cm_DC30F = settingsC.getMargin("DC30F");
    double cm_M23   = settingsC.getMargin("M23");
    double cm_M28   = settingsC.getMargin("M28");
    double cm_M24   = settingsC.getMargin("M24");
    double cm_D29   = settingsC.getMargin("D29");
    double cm_D32   = settingsC.getMargin("D32");

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
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("HR", h + cm_DC30F), labelWith("WT", w + cm_DC30F) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 2:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("HR", h + cm_DC30C), labelWith("WT", w + cm_DC30C) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 3:
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("HR", h + cm_DC30F) });
            addSection("DC30C", { labelWith("WT", w + cm_DC30C) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 4:
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("WT", w + cm_DC30F) });
            addSection("DC30C", { labelWith("HR", h + cm_DC30C) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) }); 
            break;
        case 5:
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("HR", h + cm_DC30F), labelWith("WT", w + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 6:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C) });
            addSection("DC30F", { labelWith("HR", h + cm_DC30F), labelWith("WT", w + cm_DC30F) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 7:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("WT", w + cm_DC30C) });
            addSection("DC30F", { labelWith("HR", h + cm_DC30F) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 8:
            addSection("DC30C", { labelWith("HR", h + cm_DC30C) });
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("WT", w + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 9:
            addSection("DC30C", { labelWith("WT", w + cm_DC30C) });
            addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("HR", h + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 10:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("HR", h + cm_DC30C) });
            addSection("DC30F", { labelWith("WT", w + cm_DC30F) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 11:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("HR", h + cm_DC30C) });
            addSection("DC30F", { labelWith("WT", w + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 12:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("WT", w + cm_DC30C) });
            addSection("DC30F", { labelWith("HR", h + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
        case 13:
            addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("HR", h + cm_DC30C), labelWith("WT", w + cm_DC30C) });
            addSection("DC26F", { labelWith("WB", w + cm_DC26F) });
            break;
        case 14:
            addSection("DC30C", { labelWith("HR", h + cm_DC30C), labelWith("WT", w + cm_DC30C) });
            addSection("DC30F", { labelWith("HL", h + cm_DC30F) });
            addSection("DC26C", { labelWith("WB", w + cm_DC26C) });
            break;
    }

    // Inner Sections
   if (glassOption == 1) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24), labelWith("W4", (w/4) + cm_M24),
                             labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24) });
        addSection("D29", { labelWith("W1", (w/4) + cm_D29), labelWith("W2", (w/4) + cm_D29), labelWith("W3", (w/4) + cm_D29), labelWith("W4", (w/4) + cm_D29),
                             labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29) });
    }
    else if (glassOption == 2) {
        addSection("M23", { labelWith("H1", h + cm_M23), labelWith("H2", h + cm_M23), labelWith("H3", h + cm_M23), labelWith("H4", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24), labelWith("W4", (w/4) + cm_M24),
                             labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24), labelWith("W7", (w/4) + cm_M24), labelWith("W8", (w/4) + cm_M24) });
        addSection("D29", { labelWith("W1", (w/4) + cm_D29), labelWith("W2", (w/4) + cm_D29), labelWith("W3", (w/4) + cm_D29), labelWith("W4", (w/4) + cm_D29),
                             labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29) });
        addSection("D32", { labelWith("H", h + cm_D32) });
    }
    else if (glassOption == 3) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24),
                             labelWith("W4", (w/4) + cm_M24), labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24) });
        addSection("D29", { labelWith("W1", (w/3) + cm_D29), labelWith("W2", (w/3) + cm_D29), labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29) });
    }
}

double SG_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string SG_Win_e::getLabel() const {
    return label;
}

const SectionMap& SG_Win_e::getSectionMap() const {
    return sectionMap;
}


void SG_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char SG_Win_e::getUnit() const {return unitChoice;}
double SG_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double SG_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int SG_Win_e::getCollarType() const { return collarType; }


void SG_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}
