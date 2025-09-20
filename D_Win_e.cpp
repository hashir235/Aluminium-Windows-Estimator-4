#include "D_Win_e.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

int D_Win_e::windowCount = 0; 
void D_Win_e::resetWindowCount() { windowCount = 0;}
D_Win_e::D_Win_e()
    : height(0.0), width(0.0), 
      collarType(0), winType(0), inputInInches(false) {++windowCount;}

bool D_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "D_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!inputDetails()) return false;

    return true;
}

////////////////////////////// Input Window Type Method //////////////////////////////

bool D_Win_e::selectWindowType() {
    while (true) {
        std::cout << "\n------ D_Win_e Options ------\n"
                  << "1. Single Door\n"
                  << "2. Double Door\n"
                  << "0. Back to Window Type Menu\n"
                  << "Select option: ";

        if (std::cin >> winType) {
            if (winType == 0) {
                std::cout << "⬅ Returning to Window Type Menu...\n";
                return false;  // Cancel input, go back
            }
            if (winType >= 1 && winType <= 2) {
                return true;  // Valid selection
            }
        }
        std::cout << "❌ Invalid input! Please enter a number between 1 and 5.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

////////////////////////// Input Details Method //////////////////////////

bool D_Win_e::inputDetails() {
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
        std::cout << "Enter Collar Type (1 - 8, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 8) break;
        }
        std::cout << "❌ Please enter a number between 1 and 8.\n";
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

    // 4. Width
    while (true) {
        std::cout << "Enter Right Width (0 = cancel): ";
        if (std::cin >> width) {
            if (width == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (width > 0) break;
        }
        std::cout << "❌ Right Width must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


   // Extra option 1: Add bottom (D46)
while (true) {
    std::cout << "Add bottom (D46)? (y/n): ";
    std::getline(std::cin, bottom);

    if (bottom == "y" || bottom == "Y") {
        std::cout << "✅ Bottom (D46) added!\n";
        break;
    }
    else if (bottom == "n" || bottom == "N") {
        std::cout << "❌ Bottom (D46) not added.\n";
        break;
    }
    else {
        std::cout << "⚠ Invalid input! Please enter 'y' or 'n'.\n";
    }
}

// Extra option 2: Add Tee (D46)
while (true) {
    std::cout << "Add Tee (D52)? (y/n): ";
    std::getline(std::cin, Tee);

    if (Tee == "y" || Tee == "Y") {
        std::cout << "✅ Tee (D52) added!\n";
        break;
    }
    else if (Tee == "n" || Tee == "N") {
        std::cout << "❌ Tee (D52) not added.\n";
        break;
    }
    else {
        std::cout << "⚠ Invalid input! Please enter 'y' or 'n'.\n";
    }
}


    // Convert inches to feet if needed
    if (inputInInches) {
        height /= 12.0;
        width /= 12.0;
    }

    return true;
}


void D_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;

    SettingsManagerC& settingsC = SettingsManagerC::getInstance();
    double cm_D54F = settingsC.getMargin("D54F");
    double cm_D54A = settingsC.getMargin("D54A");
    double cm_D52 = settingsC.getMargin("D52");
    double cm_D50 = settingsC.getMargin("D50");
    double cm_D46 = settingsC.getMargin("D46");

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

switch (collarType) {
        case 1:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F), 
                                 labelWith("WT", w + cm_D54F) });
            break;
        case 2:
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A), labelWith("WT", w + cm_D54A) });
            break;
        case 3:
            addSection("D54F", { labelWith("HR", h + cm_D54F), labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A) });
            break;
        case 4:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("HR", h + cm_D54F) });
            addSection("D54A", { labelWith("WT", w + cm_D54A) });
            break;
        case 5:
            addSection("D54F", { labelWith("HL", h + cm_D54F), labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("HR", h + cm_D54A) });
            break;
        case 6:
            addSection("D54F", { labelWith("HR", h + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("WT", w + cm_D54A) });
            break;
        case 7:
            addSection("D54F", { labelWith("HL", h + cm_D54F) });
            addSection("D54A", { labelWith("HR", h + cm_D54A), labelWith("WT", w + cm_D54A) });
            break;
        case 8:
            addSection("D54F", { labelWith("WT", w + cm_D54F) });
            addSection("D54A", { labelWith("HL", h + cm_D54A), labelWith("HR", h + cm_D54A) });    
            break;
    }   
    
    if(winType  == 1){ // Single Door
        
       if(bottom == "y" || bottom == "Y") {       
            addSection("D50", { labelWith("WT", w + cm_D50), labelWith("HL", h + cm_D50), labelWith("HR", h + cm_D50) });
            addSection("D46", { labelWith("WB", w + cm_D46) });
          if(Tee == "y" || Tee == "Y") {
               addSection("D52", { labelWith("Tee", w + cm_D52) });
            }
        }else if(bottom == "n" || bottom == "N") {
            addSection("D50", { labelWith("WT", w + cm_D50), labelWith("WB", w + cm_D50), labelWith("HL", h + cm_D50), labelWith("HR", h + cm_D50) });
           if(Tee == "y" || Tee == "Y") {
               addSection("D52", { labelWith("Tee", w + cm_D52) });
            }
        }
    }else if (winType == 2){ // Double Door
        
        if(bottom == "y" || bottom == "Y") {       

            addSection("D50", { labelWith("WT_l", (w/2) + cm_D50), labelWith("WT_r", (w/2) + cm_D50),
                 labelWith("HL_l", h + cm_D50), labelWith("HR_l", h + cm_D50), labelWith("HL_r", h + cm_D50), labelWith("HR_r", h + cm_D50) });
            addSection("D46", { labelWith("WB_l", (w/2) + cm_D46), labelWith("WB_r", (w/2) + cm_D46) });
        if(Tee == "y" || Tee == "Y") { //Tee
            addSection("D52", { labelWith("Tee_l", (w/2) + cm_D52), labelWith("Tee_r", (w/2) + cm_D52)});
        }

        }else if(bottom == "n" || bottom == "N") {

            addSection("D50", { labelWith("WT_l", (w/2) + cm_D50), labelWith("WT_r", (w/2) + cm_D50), labelWith("WB_l", (w/2) + cm_D50), labelWith("WB_r", (w/2) + cm_D50),
                 labelWith("HL_l", h + cm_D50), labelWith("HR_l", h + cm_D50), labelWith("HL_r", h + cm_D50), labelWith("HR_r", h + cm_D50) });   
        if(Tee == "y" || Tee == "Y") { //Tee
            addSection("D52", { labelWith("Tee_l", (w/2) + cm_D52), labelWith("Tee_r", (w/2) + cm_D52)});
        }

        }
    }
}

double D_Win_e::getAreaSqFt() const {
return height * width;
}

std::string D_Win_e::getLabel() const {
    return label;
}

const SectionMap& D_Win_e::getSectionMap() const {
    return sectionMap;
}


void D_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char D_Win_e::getSize() const {return unitChoice;}
double D_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double D_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
int D_Win_e::getCollarType() const { return collarType; }


void D_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


