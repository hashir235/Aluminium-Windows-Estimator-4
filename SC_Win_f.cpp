#include "SC_Win_f.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerD.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

void SC_Win_f::resetWindowCount() { windowCount = 0;} 
int SC_Win_f::windowCount = 0; 
SC_Win_f::SC_Win_f()
    : height(0.0), width(0.0), rightWidth(0.0), leftWidth(0.0),
      collarType(0), winType(0), inputInInches(false) {++windowCount;}


////////////////////////// Main Input Window Method ///////////////////////////

bool SC_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {
        
        label = "SCM_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    if (!selectWindowType()) return false;
    if (!selectLockType()) return false;
    if (!inputDetails()) return false;

    return true;
}


/////////////////////////// Window Type Selection ///////////////////////////

bool SC_Win_f::selectWindowType() {
    while (true) {
        std::cout << "\n------ Select Window Type ------\n"
                  << "[1] Center Fix\n"
                  << "[2] Center Slide\n"
                  << "[3] Right Fix\n"
                  << "[4] Left Fix\n"
                  << "[0] Cancel / Back\n"
                  << "Enter choice: ";

        if (std::cin >> winType) {
            if (winType == 0) {
                std::cout << "⚠ Returning to previous menu...\n";
                return false;
            }
            if (winType >= 1 && winType <= 4) {
                return true;
            }
        }

        std::cout << "❌ Invalid input! Please enter 1 to 4.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/////////////////////////// Lock Type Selection ///////////////////////////

bool SC_Win_f::selectLockType() {

    while (true) {
        std::cout << "\n------ Select Lock Type ------\n";

        switch (winType) {
            case 1: // Center Fix
                std::cout << "[1] Latch lock\n"
                          << "[2] Self lock\n"
                          << "[3] Handle/Push/Button lock\n";
                break;

            case 2: // Center Slide (no self lock)
                std::cout << "[1] Latch lock\n"
                          << "[2] Handle/Push/Button lock\n";
                break;

            case 3: // Right Fix
            case 4: // Left Fix
                std::cout << "[1] Latch lock\n"
                          << "[2] Self lock\n"
                          << "[3] Handle/Push/Button lock\n";
                break;
        }

        std::cout << "Enter choice (0 = cancel): ";

        if (std::cin >> lockChoice) {
            if (lockChoice == 0) {
                std::cout << "⚠ Lock selection cancelled!\n";
                return false;
            }

            // ✅ Validation
            if (winType == 2 && (lockChoice >= 1 && lockChoice <= 2)) {
                lockType = lockChoice;
                return true;
            } 
            else if (winType != 2 && (lockChoice >= 1 && lockChoice <= 3)) {
                lockType = lockChoice;
                return true;
            }
        }

        std::cout << "❌ Invalid choice! Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/////////////////////////// Main Input Window Method ///////////////////////////

bool SC_Win_f::inputDetails() {
    
    // 3️⃣ Collar Type
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

    // 4️⃣ Height (cm ) / feet)
    while (true) {
        std::cout << "Enter Height in cm (0 = cancel): ";
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

    // 5️⃣ Width (cm ) / feet)
    while (true) {
        std::cout << "Enter Width in cm (0 = cancel): ";
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

    // ✅ Final confirmation
    std::cout << "\n✅ Window successfully created!\n";
    return true;
}

///////////////////////// Calculations Method /////////////////////////
void SC_Win_f::calculateSections() {
    sectionMap.clear();

    double h = height;
    double wr = rightWidth;
    double wl = leftWidth;
    double feet = 30.48; // cm to feet
     
    // 🔹 Fabricator Cutting Margin from SettingsManagerD
    SettingsManagerD& settingsD = SettingsManagerD::getInstance();
    double cm = settingsD.getCuttingMargin();   // default 1.2 agar file me na mile

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

    // Collar Type Sections
    if (collarType == 1) {
        addSection("DC30F", { labelWith("HL", ((h + 6)+ cm ) / feet), labelWith("HR", ((h + 6)+ cm ) / feet),
                               labelWith("WT_l", ((wl + 6)+ cm ) / feet), labelWith("WT_r", ((wr + 6)+ cm ) / feet) });
        addSection("DC26F", { labelWith("WB_l", ((wl + 6)+ cm ) / feet), labelWith("WB_r", ((wr + 6)+ cm ) / feet) });
    }
    else if (collarType == 2) {
        addSection("DC30C", { labelWith("HL", (h + cm ) / feet), labelWith("HR", (h + cm ) / feet),
                               labelWith("WT_l", (wl + cm ) / feet), labelWith("WT_r", (wr + cm ) / feet) });
        addSection("DC26C", { labelWith("WB_l", (wl + cm ) / feet), labelWith("WB_r", (wr + cm ) / feet) });
    }

    // Inner Sections - Corner Window
    if (winType == 1) {       // Center Fix
            if(lockChoice == 1){

                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wr - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wr - 11.5)/ 2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("WL1", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WL2", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), 
                                    labelWith("WR1", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WR2", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet) });
      
            }else if(lockChoice == 2){
           
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wr - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wr - 11.5)/ 2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("WL1", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WL2", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), 
                                    labelWith("WR1", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WR2", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet) });

            }else if(lockChoice == 3){
    
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2)+ cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2)+ cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WR1", (((wr - 11.5)/ 2)+ cm) / feet), labelWith("WR2", (((wr - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WR3", (((wr - 11.5)/ 2)+ cm) / feet), labelWith("WR4", (((wr - 11.5)/ 2)+ cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("WL1", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WL2", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), 
                                    labelWith("WR1", ((((wr - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WR2", ((((wr - 11.5)/ 2)+ 5.5)+ cm )/ feet) });

        }
      
    }
    else if (winType == 2) {      // Center Slide
            if(lockChoice == 1){

                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wr - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wr - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wr - 11.5)/ 2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("WL1", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WL2", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), 
                                    labelWith("WR1", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WR2", ((((wr - 11.5)/ 2)+ 8.5)+ cm )/ feet) });
      
            }else if(lockChoice == 2){
    
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2)+ cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2)+ cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WR1", (((wr - 11.5)/ 2)+ cm) / feet), labelWith("WR2", (((wr - 11.5)/ 2)+ cm) / feet),
                                    labelWith("WR3", (((wr - 11.5)/ 2)+ cm) / feet), labelWith("WR4", (((wr - 11.5)/ 2)+ cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet),
                                    labelWith("WL1", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WL2", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), 
                                    labelWith("WR1", ((((wr - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WR2", ((((wr - 11.5)/ 2)+ 5.5)+ cm )/ feet) });

        }
    }
    else if (winType == 3) {   // Right Fix
        if(lockChoice == 1){

                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", ((wr - 6.2) + cm) / feet), labelWith("WR2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WL", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WL", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet)
                                  });
      
            }else if(lockChoice == 2){
           
                // Self lock panding!
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", ((wr - 6.2) + cm) / feet), labelWith("WR2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WL", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WL", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet)
                                  });

            }else if(lockChoice == 3){
    
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WL1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WL4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR1", ((wr - 6.2) + cm) / feet), labelWith("WR2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WL", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WL", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet)
                                  });
        }
      
    }
    else if (winType == 4) {   // Left Fix
     
        if(lockChoice == 1){

                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WR1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL1", ((wr - 6.2) + cm) / feet), labelWith("WL2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WR", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WR", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet)
                                  });
      
            }else if(lockChoice == 2){
           
                // Self lock panding!
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WR1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL1", ((wr - 6.2) + cm) / feet), labelWith("WL2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WR", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet), labelWith("WR", ((((wl - 11.5)/ 2)+ 8.5)+ cm )/ feet)
                                  });

            }else if(lockChoice == 3){
    
                addSection("M23", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M28", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet) });
                addSection("M24", { labelWith("WR1", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR2", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WR3", (((wl - 11.5)/ 2) + cm) / feet), labelWith("WR4", (((wl - 11.5)/ 2) + cm) / feet),
                                    labelWith("WL1", ((wr - 6.2) + cm) / feet), labelWith("WL2", ((wr - 6.2) + cm) / feet) });
                addSection("D29", { labelWith("H", ((h - 4.2)+ cm ) / feet), labelWith("H", ((h - 4.2)+ cm ) / feet), 
                                    labelWith("WR", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet), labelWith("WR", ((((wl - 11.5)/ 2)+ 5.5)+ cm )/ feet)
                                  });
      
        }
    }
}

double SC_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}

std::string SC_Win_f::getLabel() const {
    return label;
}

const SectionMap& SC_Win_f::getSectionMap() const {
    return sectionMap;
}


void SC_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}

double SC_Win_f::getHeight() const { return height;}
double SC_Win_f::getRightWidth() const { return rightWidth; }
double SC_Win_f::getLeftWidth() const { return leftWidth; }
int SC_Win_f::getCollarType() const { return collarType; }


void SC_Win_f::sendToEstimator() {
    std::cout << "[SlideWindow] Warning: No estimator provided in sendToEstimator().\n";
}


