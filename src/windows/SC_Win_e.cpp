#include "SC_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

void SC_Win_e::resetWindowCount() { windowCount = 0;} 
int SC_Win_e::windowCount = 0; 
SC_Win_e::SC_Win_e()
    : height(0.0), width(0.0), rightWidth(0.0), leftWidth(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}


bool SC_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "SC_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!inputDetails()) return false;

    return true;
}

////////////////////////////// Input Window Type Method //////////////////////////////

bool SC_Win_e::selectWindowType() {
    while (true) {
        std::cout << "\n------ SC_Win_e Options ------\n"
                  << "1. Center Fix\n"
                  << "2. Center Slide\n"
                  << "3. Right Fix\n"
                  << "4. Left Fix\n"
                  << "5. Back to Window Type Menu\n"
                  << "Select option: ";

        if (std::cin >> winType) {
            if (winType == 5) {
                std::cout << "⬅ Returning to Window Type Menu...\n";
                return false;  // Cancel input, go back
            }
            if (winType >= 1 && winType <= 4) {
                return true;  // Valid selection
            }
        }
        std::cout << "❌ Invalid input! Please enter a number between 1 and 5.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

////////////////////////// Input Details Method //////////////////////////

bool SC_Win_e::inputDetails() {
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

///////////////////////// Calculations Method /////////////////////////
void SC_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double wr = rightWidth;
    double wl = leftWidth;
     
    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_DC30C = settingsC.getMargin("DC30C");
    double cm_DC26F = settingsC.getMargin("DC26F");
    double cm_DC26C = settingsC.getMargin("DC26C");
    double cm_DC30F = settingsC.getMargin("DC30F");
    double cm_M23   = settingsC.getMargin("M23");
    double cm_M28   = settingsC.getMargin("M28");
    double cm_M24   = settingsC.getMargin("M24");
    double cm_D29   = settingsC.getMargin("D29");

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
        addSection("DC30F", { labelWith("HL", h + cm_DC30F), labelWith("HR", h + cm_DC30F),
                               labelWith("WT_l", wl + cm_DC30F), labelWith("WT_r", wr + cm_DC30F) });
        addSection("DC26F", { labelWith("WB_l", wl + cm_DC26F), labelWith("WB_r", wr + cm_DC26F) });
    }
    else if (collarType == 2) {
        addSection("DC30C", { labelWith("HL", h + cm_DC30C), labelWith("HR", h + cm_DC30C),
                               labelWith("WT_l", wl + cm_DC30C), labelWith("WT_r", wr + cm_DC30C) });
        addSection("DC26C", { labelWith("WB_l", wl + cm_DC26C), labelWith("WB_r", wr + cm_DC26C) });
    }

    // Inner Sections - Corner Window
    if (winType == 1) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WL1", (wl/2) + cm_M24), labelWith("WL2", (wl/2) + cm_M24), labelWith("WL3", (wl/2) + cm_M24), labelWith("WL4", (wl/2) + cm_M24),
                             labelWith("WR1", (wr/2) + cm_M24), labelWith("WR2", (wr/2) + cm_M24), labelWith("WR3", (wr/2) + cm_M24), labelWith("WR4", (wr/2) + cm_M24) });
        addSection("D29", { labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29),
                             labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29), labelWith("WR1", (wr/2) + cm_D29), labelWith("WR2", (wr/2) + cm_D29) });
    }
    else if (winType == 2) {
        addSection("M23", { labelWith("H1", h + cm_M23), labelWith("H2", h + cm_M23), labelWith("H3", h + cm_M23), labelWith("H4", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WL1", (wl/2) + cm_M24), labelWith("WL2", (wl/2) + cm_M24), labelWith("WL3", (wl/2) + cm_M24), labelWith("WL4", (wl/2) + cm_M24),
                             labelWith("WR1", (wr/2) + cm_M24), labelWith("WR2", (wr/2) + cm_M24), labelWith("WR3", (wr/2) + cm_M24), labelWith("WR4", (wr/2) + cm_M24) });
        addSection("D29", { labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29),
                             labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29), labelWith("WR1", (wr/2) + cm_D29), labelWith("WR2", (wr/2) + cm_D29) });
    }
    else if (winType == 3) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WL1", (wl/2) + cm_M24), labelWith("WL2", (wl/2) + cm_M24), labelWith("WL3", (wl/2) + cm_M24), labelWith("WL4", (wl/2) + cm_M24),
                             labelWith("WR1", wr + cm_M28), labelWith("WR2", wr + cm_M28) });
        addSection("D29", { labelWith("H", h + cm_D29), labelWith("H", h + cm_D29),
                             labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29)});
    }
    else if (winType == 4) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WR1", (wr/2) + cm_M24), labelWith("WR2", (wr/2) + cm_M24), labelWith("WR3", (wr/2) + cm_M24), labelWith("WR4", (wr/2) + cm_M24),
                             labelWith("WL1", wl + cm_M28), labelWith("WL2", wl + cm_M28) });
        addSection("D29", { labelWith("H", h + cm_D29), labelWith("H", h + cm_D29),
                             labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29) });
    }
}

double SC_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string SC_Win_e::getLabel() const {
    return label;
}

const SectionMap& SC_Win_e::getSectionMap() const {
    return sectionMap;
}


void SC_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char SC_Win_e::getUnit() const {return unitChoice;}
double SC_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double SC_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
double SC_Win_e::getRightWidth() const { if (inputInInches) {return rightWidth * 12.0;} return rightWidth; }
double SC_Win_e::getLeftWidth() const { if (inputInInches) {return leftWidth * 12.0;} return leftWidth; }
int SC_Win_e::getCollarType() const { return collarType; }


void SC_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


