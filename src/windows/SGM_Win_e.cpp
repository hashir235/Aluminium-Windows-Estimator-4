#include "SGM_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


void SGM_Win_e::resetWindowCount() { windowCount = 0;} 
int SGM_Win_e::windowCount = 0; 
SGM_Win_e::SGM_Win_e() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    inputInInches = false;
    ++windowCount;
}

// Returns true if data entered successfully, false if cancelled
bool SGM_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "SGM_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectGlassOption()) return false;
    if (!inputSizeAndCollar()) return false;

    return true;
}

bool SGM_Win_e::selectGlassOption() {
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
        std::cout << " Invalid input! Please enter 1, 2, or 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

bool SGM_Win_e::inputSizeAndCollar() {
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
        std::cout << " Invalid input! Please enter 'i' or 'f'.\n";
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
        std::cout << " Please enter a number between 1 and 14.\n";
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
        std::cout << " Height must be a positive number.\n";
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
        std::cout << " Width must be a positive number.\n";
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


void SGM_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;
  
    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_M30 = settingsC.getMargin("M30");
    double cm_M26F = settingsC.getMargin("M26F");
    double cm_M26 = settingsC.getMargin("M26");
    double cm_M30F = settingsC.getMargin("M30F");
    double cm_M23 = settingsC.getMargin("M23");
    double cm_M28 = settingsC.getMargin("M28");
    double cm_M24 = settingsC.getMargin("M24");
    double cm_D29 = settingsC.getMargin("D29");
    double cm_D32 = settingsC.getMargin("D32");

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
   if (glassOption == 1) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24), labelWith("W4", (w/4) + cm_M24),
                             labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24) });
        //addSection("D29", { labelWith("W1", (w/4) + cm_D29), labelWith("W2", (w/4) + cm_D29), labelWith("W3", (w/4) + cm_D29), labelWith("W4", (w/4) + cm_D29),
        //                     labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29) });
    }
    else if (glassOption == 2) {
        addSection("M23", { labelWith("H1", h + cm_M23), labelWith("H2", h + cm_M23), labelWith("H3", h + cm_M23), labelWith("H4", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24), labelWith("W4", (w/4) + cm_M24),
                             labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24), labelWith("W7", (w/4) + cm_M24), labelWith("W8", (w/4) + cm_M24) });
        //addSection("D29", { labelWith("W1", (w/4) + cm_D29), labelWith("W2", (w/4) + cm_D29), labelWith("W3", (w/4) + cm_D29), labelWith("W4", (w/4) + cm_D29),
        //                     labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29) });
        addSection("D32", { labelWith("H", h + cm_D32) });
    }
    else if (glassOption == 3) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("W1", (w/4) + cm_M24), labelWith("W2", (w/4) + cm_M24), labelWith("W3", (w/4) + cm_M24),
                             labelWith("W4", (w/4) + cm_M24), labelWith("W5", (w/4) + cm_M24), labelWith("W6", (w/4) + cm_M24) });
        //addSection("D29", { labelWith("W1", (w/3) + cm_D29), labelWith("W2", (w/3) + cm_D29), labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29) });
    }
}

double SGM_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string SGM_Win_e::getLabel() const {
    return label;
}

const SectionMap& SGM_Win_e::getSectionMap() const {
    return sectionMap;
}


void SGM_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char SGM_Win_e::getUnit() const {return unitChoice;}
double SGM_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double SGM_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int SGM_Win_e::getCollarType() const { return collarType; }


void SGM_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}
