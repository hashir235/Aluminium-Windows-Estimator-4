#include "A_Win_e.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "Margins_Settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int A_Win_e::windowCount = 0; 
void A_Win_e::resetWindowCount() { windowCount = 0;}
A_Win_e::A_Win_e()
    : height(0.0), width(0.0), arch(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}

bool A_Win_e::inputWindowData(bool isEditing) {
    if (!isEditing){      
        label = "A_Win_e #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!inputDetails()) return false;

    return true;
}

////////////////////////////// Input Window Type Method //////////////////////////////

bool A_Win_e::selectWindowType() {
    while (true) {
        std::cout << "\n------ A_Win_e Options ------\n"
                  << "1. Rectangle Arch\n"
                  << "2. Round Arch\n"
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

bool A_Win_e::inputDetails() {
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

    if(winType == 1) {

    // 2. Collar type of Rectangle Arch
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

    } else if (winType == 2) {
    // 3. Collar type of Round Arch
    while (true) {
        std::cout << "Enter Collar Type (1 - 2, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 2) break;
        }
        std::cout << "❌ Please enter a number between 1 and 8.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 6. Arch
    while (true) {
        std::cout << "Enter Right Width (0 = cancel): ";
        if (std::cin >> arch) {
            if (arch == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (arch > 0) break;
        }
        std::cout << "❌ Right Width must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    }
    // 5. Height
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

    // 6. Width
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


    // Convert inches to feet if needed
    if (inputInInches) {
        height /= 12.0;
        width /= 12.0;
        arch /= 12.0; 
    }

    return true;
}


void A_Win_e::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;
    double ar = arch;

    Margins_Settings& settingsC = Margins_Settings::getInstance();
    double cm_D51F = settingsC.getMargin("D51F");
    double cm_D51A = settingsC.getMargin("D51A");
    double cm_D41  = settingsC.getMargin("D41");

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

if(winType  == 1){ // Rectangle Arch

switch (collarType) {
        case 1:
            addSection("D51F", { labelWith("HL", h + cm_D51F), labelWith("HR", h + cm_D51F), labelWith("WT", w + cm_D51F) });
            addSection("D51A", { labelWith("WB", w + cm_D51A) });
            break;
        case 2:
            addSection("D51A", { labelWith("HL", h + cm_D51A), labelWith("HR", h + cm_D51A), labelWith("WT", w + cm_D51A), labelWith("WB", w + cm_D51A) }); 
            break;
        case 3:
            addSection("D51F", { labelWith("HR", h + cm_D51F), labelWith("WT", w + cm_D51F) });
            addSection("D51A", { labelWith("WB", w + cm_D51A), labelWith("HL", h + cm_D51A) });
            break;
        case 4:
            addSection("D51F", { labelWith("HL", h + cm_D51F), labelWith("HR", h + cm_D51F)  });
            addSection("D51A", { labelWith("WB", w + cm_D51A), labelWith("WT", w + cm_D51A) }); 
            break;
        case 5:
            addSection("D51F", { labelWith("HL", h + cm_D51F), labelWith("WT", w + cm_D51F) });
            addSection("D51A", { labelWith("WB", w + cm_D51A),  labelWith("HR", h + cm_D51A)});
            break;
        case 6:
            addSection("D51F", {  labelWith("WT", w + cm_D51F) });
            addSection("D51A", { labelWith("WB", w + cm_D51A), labelWith("HR", h + cm_D51A),labelWith("HL", h + cm_D51A) });
            break;
        case 7:
            addSection("D51F", { labelWith("HL", h + cm_D51F), });
            addSection("D51A", { labelWith("WB", w + cm_D51A), labelWith("HR", h + cm_D51A), labelWith("WT", w + cm_D51A) });
            break;
        case 8:
            addSection("D51F", { labelWith("HR", h + cm_D51F), });
            addSection("D51A", { labelWith("WB", w + cm_D51A), labelWith("WT", w + cm_D51A), labelWith("HL", h + cm_D51A) });
            break;
    }   
    
     addSection("D41", { labelWith("HL", h + cm_D41), labelWith("HR", h + cm_D41), labelWith("WT", w + cm_D41), labelWith("WB", w + cm_D41) });
            

    }else if (winType == 2){ // Round Arch

     if(collarType == 1){
     addSection("D51F", { labelWith("Arch", arch + 1) });
     addSection("D51A", { labelWith("WB", w + cm_D51A) });

     addSection("D41", { labelWith("Arch", arch + 1), labelWith("WB", w + cm_D41) });  
     } else{

     addSection("D51A", { labelWith("Arch", arch + 1), labelWith("WB", w + cm_D51A) });

     addSection("D41", { labelWith("Arch", arch + 1), labelWith("WB", w + cm_D41) });            
     }
    }
}

double A_Win_e::getAreaSqFt() const {
    return height * width;
}

std::string A_Win_e::getLabel() const {
    return label;
}

const SectionMap& A_Win_e::getSectionMap() const {
    return sectionMap;
}


void A_Win_e::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char A_Win_e::getSize() const {return unitChoice;}
double A_Win_e::getHeight() const { if (inputInInches) {return height * 12.0;} return height;}
double A_Win_e::getWidth() const {if (inputInInches){return width * 12.0;} return width;}
double A_Win_e::getArch() const { if (inputInInches){return arch * 12.0;} return arch; }
int A_Win_e::getCollarType() const { return collarType; }


void A_Win_e::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


