#include "D_Win_f.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "fabricator_settings.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

int D_Win_f::windowCount = 0; 
void D_Win_f::resetWindowCount() { windowCount = 0;}
D_Win_f::D_Win_f()
    : height(0.0), width(0.0), 
      collarType(0), winType(0), inputInInches(false) {++windowCount;}

bool D_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "D_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!inputDetails()) return false;

    return true;
}

////////////////////////////// Input Window Type Method //////////////////////////////

bool D_Win_f::selectWindowType() {
    while (true) {
        std::cout << "\n------ D_Win_f Options ------\n"
                  << "1. Single Door\n"
                  << "2. Double Door\n"
                  << "0. Back to Window Type Menu\n"
                  << "Select option: ";

        if (std::cin >> winType) {
            if (winType == 0) {
                std::cout << " Returning to Window Type Menu...\n";
                return false;  // Cancel input, go back
            }
            if (winType >= 1 && winType <= 2) {
                return true;  // Valid selection
            }
        }
        std::cout << " Invalid input! Please enter a number between 1 and 5.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

////////////////////////// Input Details Method //////////////////////////

bool D_Win_f::inputDetails() {
   
    // 1. Collar type
    while (true) {
        std::cout << "Enter Collar Type (1 - 8, 0 = cancel): ";
        if (std::cin >> collarType) {
            if (collarType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (collarType >= 1 && collarType <= 8) break;
        }
        std::cout << " Please enter a number between 1 and 8.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 2. Height
    while (true) {
        std::cout << "Enter Height in cm (0 = cancel): ";
        if (std::cin >> height) {
            if (height == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (height > 0) break;
        }
        std::cout << " Height must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 3. Width
    while (true) {
        std::cout << "Enter Right Width in cm (0 = cancel): ";
        if (std::cin >> width) {
            if (width == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (width > 0) break;
        }
        std::cout << " Right Width must be positive.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


   // Extra option 1: Add bottom (D46)
while (true) {
    std::cout << "Add bottom (D46)? (y/n): ";
    std::getline(std::cin, bottom);

    if (bottom == "y" || bottom == "Y") {
        std::cout << " Bottom (D46) added!\n";
        break;
    }
    else if (bottom == "n" || bottom == "N") {
        std::cout << " Bottom (D46) not added.\n";
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
        std::cout << " Tee (D52) added!\n";
        break;
    }
    else if (Tee == "n" || Tee == "N") {
        std::cout << " Tee (D52) not added.\n";
        break;
    }
    else {
        std::cout << "⚠ Invalid input! Please enter 'y' or 'n'.\n";
    }
}

    return true;
}


void D_Win_f::calculateSections() {
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
        for (const auto& [lbl, val] : values) {
            converted.emplace_back(val, lbl);
        }
        sectionMap[settings.getSectionName(key)] = converted;
    };

switch (collarType) {
        case 1:
            addSection("D54F", { labelWith("HL", ((h + 6) + cm) / feet), labelWith("HR", ((h + 6) + cm) / feet), 
                                 labelWith("WT", ((w + 6) + cm) / feet) });
            break;
        case 2:
            addSection("D54A", { labelWith("HL", ((h) + cm) / feet), labelWith("HR", ((h) + cm) / feet), labelWith("WT", ((w) + cm) / feet) });
            break;
        case 3:
            addSection("D54F", { labelWith("HR",((h + 6) + cm) / feet), labelWith("WT",((w + 6) + cm) / feet) });
            addSection("D54A", { labelWith("HL", ((h) + cm) / feet) });
            break;
        case 4:
            addSection("D54F", { labelWith("HL", ((h + 6) + cm) / feet), labelWith("HR", ((h + 6) + cm) / feet) });
            addSection("D54A", { labelWith("WT", ((w) + cm) / feet) });
            break;
        case 5:
            addSection("D54F", { labelWith("HL", ((h + 6) + cm) / feet), labelWith("WT", ((w + 6) + cm) / feet) });
            addSection("D54A", { labelWith("HR", ((h) + cm) / feet) });
            break;
        case 6:
            addSection("D54F", { labelWith("HR", ((h + 6) + cm) / feet) });
            addSection("D54A", { labelWith("HL", ((h) + cm) / feet), labelWith("WT", ((w) + cm) / feet) });
            break;
        case 7:
            addSection("D54F", { labelWith("HL", ((h + 6) + cm) / feet) });
            addSection("D54A", { labelWith("HR", ((h) + cm) / feet), labelWith("WT", ((w) + cm) / feet) });
            break;
        case 8:
            addSection("D54F", { labelWith("WT", ((w + 6) + cm) / feet) });
            addSection("D54A", { labelWith("HL", ((h) + cm) / feet), labelWith("HR", ((h) + cm) / feet) });    
            break;
    }   
    
    if(winType  == 1){ // Single Door
        
       if(bottom == "y" || bottom == "Y") {       
            addSection("D50", { labelWith("WT", ((w - 3.4)+ cm)/ feet), labelWith("HL", (((h - 3.4) )+ cm)/ feet), labelWith("HR", (((h - 3.4) )+ cm)/ feet) });
            addSection("D46", { labelWith("WB", (((w - 3.4)- 13)+ cm)/ feet) });
          if(Tee == "y" || Tee == "Y") {

            addSection("D52", { labelWith("Tee", (((w - 3.4)- 13)+ cm)/ feet) });

            }
        }else if(bottom == "n" || bottom == "N") {

            addSection("D50", { labelWith("WT", (((w - 3.4) )+ cm)/ feet), labelWith("WB", ((w - 3.4)+ cm)/ feet), 
                                labelWith("HL", (((h - 3.4) )+ cm)/ feet), labelWith("HR", (((h - 3.4) )+ cm)/ feet) });

           if(Tee == "y" || Tee == "Y") {

            addSection("D52", { labelWith("Tee", (((w - 3.4)- 13)+ cm)/ feet) });

            }
        }
    }else if (winType == 2){ // Double Door
        
        if(bottom == "y" || bottom == "Y") {       

            addSection("D50", { labelWith("WT_l", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet), labelWith("WT_r", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet),
                                labelWith("HL_l", (((h - 3.4) )+ cm)/ feet), labelWith("HR_l", (((h - 3.4) )+ cm)/ feet), 
                                labelWith("HL_r", (((h - 3.4) )+ cm)/ feet), labelWith("HR_r", (((h - 3.4) )+ cm)/ feet) });
            addSection("D46", { labelWith("WB_l", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet), labelWith("WB_r", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet)});

        if(Tee == "y" || Tee == "Y") { //Tee

            addSection("D52", { labelWith("Tee_l", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet), labelWith("Tee_r", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet)});
        }

        }else if(bottom == "n" || bottom == "N") {

            addSection("D50", { labelWith("WT_l", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet), labelWith("WT_r", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet),
                                labelWith("WB_l", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet), labelWith("WB_r", ((((w - 3.4)+ 0.8)/ 2)+ cm)/ feet),
                                labelWith("HL_l", (((h - 3.4) )+ cm)/ feet), labelWith("HR_l", (((h - 3.4) )+ cm)/ feet), 
                                labelWith("HL_r", (((h - 3.4) )+ cm)/ feet), labelWith("HR_r", (((h - 3.4) )+ cm)/ feet) });   
        if(Tee == "y" || Tee == "Y") { //Tee

            addSection("D52", { labelWith("Tee_l", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet), labelWith("Tee_r", (((((w - 3.4)+ 0.8)/ 2)- 13)+ cm)/ feet)});
        }

        }
    }
}

double D_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}


std::string D_Win_f::getLabel() const {
    return label;
}

const SectionMap& D_Win_f::getSectionMap() const {
    return sectionMap;
}


void D_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

char D_Win_f::getSize() const {return unitChoice;}
double D_Win_f::getHeight() const { return height;}
double D_Win_f::getWidth() const { return width;}
int D_Win_f::getCollarType() const { return collarType; }


void D_Win_f::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


