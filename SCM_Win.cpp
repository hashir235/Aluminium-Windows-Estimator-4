#include "SCM_Win.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


void SCM_Win::resetWindowCount() { windowCount = 0;} 
int SCM_Win::windowCount = 0; 
SCM_Win::SCM_Win()
    : height(0.0), width(0.0), rightWidth(0.0), leftWidth(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}


bool SCM_Win::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "SCM_Win #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!inputDetails()) return false;

    return true;
}

bool SCM_Win::selectWindowType() {
    while (true) {
        std::cout << "\n------ SCM_Win Options ------\n"
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


bool SCM_Win::inputDetails() {
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

/////////////////////////// CALCULATIONS ///////////////////////////
void SCM_Win::calculateSections() {
    sectionMap.clear();

    double h = height;
    double wr = rightWidth;
    double wl = leftWidth;

    SettingsManagerC& settingsC = SettingsManagerC::getInstance();
    double cm_M30   = settingsC.getMargin("M30");
    double cm_M30F  = settingsC.getMargin("M30F");
    double cm_M26   = settingsC.getMargin("M26");
    double cm_M26F  = settingsC.getMargin("M26F");
    double cm_M23   = settingsC.getMargin("M23");
    double cm_M28   = settingsC.getMargin("M28");
    double cm_M24   = settingsC.getMargin("M24");
    double cm_D29   = settingsC.getMargin("D29");

    SettingsManagerA& settings = SettingsManagerA::getInstance();

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
        addSection("M26F", { labelWith("HL", h + cm_M26F), labelWith("HR", h + cm_M26F),
                               labelWith("WT_l", wl + cm_M26F), labelWith("WT_r", wr + cm_M26F) });
        addSection("M30F", { labelWith("WB_l", wl + cm_M30F), labelWith("WB_r", wr + cm_M30F) });
    }
    else if (collarType == 2) {
        addSection("M30", { labelWith("HL", h + cm_M30), labelWith("HR", h + cm_M30),
                               labelWith("WT_l", wl + cm_M30), labelWith("WT_r", wr + cm_M30) });
        addSection("M26", { labelWith("WB_l", wl + cm_M26), labelWith("WB_r", wr + cm_M26) });
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
       // addSection("D29", { labelWith("H1", h + cm_D29), labelWith("H2", h + cm_D29), labelWith("H3", h + cm_D29), labelWith("H4", h + cm_D29),
       //                      labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29), labelWith("WR1", (wr/2) + cm_D29), labelWith("WR2", (wr/2) + cm_D29) });
    }
    else if (winType == 3) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WL1", (wl/2) + cm_M24), labelWith("WL2", (wl/2) + cm_M24), labelWith("WL3", (wl/2) + cm_M24), labelWith("WL4", (wl/2) + cm_M24),
                             labelWith("WR1", wr + cm_M28), labelWith("WR2", wr + cm_M28) });
       // addSection("D29", { labelWith("H", h + cm_D29), labelWith("H", h + cm_D29),
       //                      labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29)});
    }
    else if (winType == 4) {
        addSection("M23", { labelWith("H", h + cm_M23), labelWith("H", h + cm_M23) });
        addSection("M28", { labelWith("H", h + cm_M28), labelWith("H", h + cm_M28) });
        addSection("M24", { labelWith("WR1", (wr/2) + cm_M24), labelWith("WR2", (wr/2) + cm_M24), labelWith("WR3", (wr/2) + cm_M24), labelWith("WR4", (wr/2) + cm_M24),
                             labelWith("WL1", wl + cm_M28), labelWith("WL2", wl + cm_M28) });
        //addSection("D29", { labelWith("H", h + cm_D29), labelWith("H", h + cm_D29),
        //                     labelWith("WL1", (wl/2) + cm_D29), labelWith("WL2", (wl/2) + cm_D29) });
    }
}

double SCM_Win::getAreaSqFt() const {
    return height * width;
}

std::string SCM_Win::getLabel() const {
    return label;
}

const SectionMap& SCM_Win::getSectionMap() const {
    return sectionMap;
}


void SCM_Win::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char SCM_Win::getSize() const {return unitChoice;}
double SCM_Win::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double SCM_Win::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
double SCM_Win::getRightWidth() const { if (inputInInches) {return rightWidth * 12.0;} return rightWidth; }
double SCM_Win::getLeftWidth() const { if (inputInInches) {return leftWidth * 12.0;} return leftWidth; }
int SCM_Win::getCollarType() const { return collarType; }


void SCM_Win::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


