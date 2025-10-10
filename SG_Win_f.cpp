#include "SG_Win_f.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include "SettingsManagerD.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int SG_Win_f::windowCount = 0; 
void SG_Win_f::resetWindowCount() { windowCount = 0;}
SG_Win_f::SG_Win_f() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    lockType = 0;
    ++windowCount;
}


bool SG_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {  
        label = "SG_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    // ======================================
    // Step 1: Window Type Selection
    // ======================================
    while (true) {
        std::cout << "\n-- Select Window Type --\n";
        std::cout << "[1] Center Fix             | |   | |\n";
        std::cout << "[2] Center Slide           | | | | |\n";
        std::cout << "[3] Equal Three Glass Part  | | | | \n";
        std::cout << "Enter choice (1-3, 0 = cancel): ";

        if (std::cin >> windowType) {
            if (windowType == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (windowType >= 1 && windowType <= 3) break;
        }
        std::cout << "❌ Invalid choice! Please enter 1, 2, or 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // ======================================
    // Step 2: Lock Type Selection (depends on windowType)
    // ======================================
    int lockChoice = 0;
    while (true) {
        std::cout << "\n-- Select Lock Type --\n";
        if (windowType == 1) { // Center Fix
            std::cout << "[1] Latch lock\n";
            std::cout << "[2] Self lock\n";
            std::cout << "[3] Handle/Push/Button lock\n";
        } 
        else if (windowType == 2) { // Center Slide
            std::cout << "[1] Latch lock\n";
            std::cout << "[2] Handle/Push/Button lock\n";
        } 
        else if (windowType == 3) { // Equal three glass part
            std::cout << "[1] Latch lock\n";
            std::cout << "[2] Self lock\n";
            std::cout << "[3] Handle/Push/Button lock\n";
        }

        std::cout << "Enter choice (0 = cancel): ";

        if (std::cin >> lockChoice) {
            if (lockChoice == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }

            // Validation depends on windowType
            if (windowType == 1 && (lockChoice >= 1 && lockChoice <= 3)) {
                lockType = lockChoice;
                break;
            }
            if (windowType == 2 && (lockChoice >= 1 && lockChoice <= 2)) {
                lockType = lockChoice;
                break;
            }
            if (windowType == 3 && (lockChoice >= 1 && lockChoice <= 3)) {
                lockType = lockChoice;
                break;
            }
        }

        std::cout << "❌ Invalid choice for this window type!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // ======================================
    // Step 3: Collar Type
    // ======================================
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

    // ======================================
    // Step 4: Height
    // ======================================
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

    // ======================================
    // Step 5: Width
    // ======================================
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

    return true; // ✅ Input complete
}




void SG_Win_f::calculateSections() {
    sectionMap.clear();

    double h = height;
    double w = width;
    double feet = 30.48;

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

    // 🔹 Collar type based outer sections
    switch (collarType) {
        case 1:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 2:
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 3:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 4:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HR", ((h) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) }); 
            break;
        case 5:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 6:
            addSection("DC30F", { labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 7:
            addSection("DC30F", { labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 8:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HR", ((h) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 9:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 10:
            addSection("DC30F", { labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 11:
            addSection("DC30F", { labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 12:
            addSection("DC30F", { labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 13:
            addSection("DC30C", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 14:
            addSection("DC30F", { labelWith("HL", ((h + 6) + cm)/feet) });
            addSection("DC30C", { labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("DC26C", { labelWith("WB", ((w) + cm)/feet) });
            break;
    }

    if(windowType == 1){

         if (lockType == 1)
    {
    // 🔹 Inner Sections for Latch lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1s", (((((w - 20.32)/2)- 5.08)/2)+ cm) / feet), labelWith("W2f", ((((w - 20.32)/2)+5.08)+ cm) / feet),
                        labelWith("W3s", (((((w - 20.32)/2)- 5.08)/2)+ cm) / feet), labelWith("W3s", (((((w - 20.32)/2)- 5.08)/2)+ cm) / feet),
                        labelWith("W5f", ((((w - 20.32)/2)+5.08)+ cm) / feet), labelWith("W6s", (((((w - 20.32)/2)- 5.08)/2)+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((((w - 20.32)/2)-5.08 )/2 )+ 8.5)+ cm)/ feet), labelWith("WB", ((((((w - 20.32)/2)-5.08 )/2 )+ 8.5)+ cm)/ feet),
                        labelWith("WT", ((((((w - 20.32)/2)-5.08 )/2 )+ 8.5)+ cm)/ feet), labelWith("WB", ((((((w - 20.32)/2)-5.08 )/2 )+ 8.5)+ cm)/ feet)
                      });
                    
    }else if (lockType == 2)
    {
    // 🔹 Inner Sections for Self lock 
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1s", (((w - 20)/4 )+ cm) / feet), labelWith("W2f", (((w - 20)/2 )+ cm) / feet),
                        labelWith("W3s", (((w - 20)/4 )+ cm) / feet), labelWith("W4s", (((w - 20)/4 )+ cm) / feet),
                        labelWith("W5f", (((w - 20)/2 )+ cm) / feet), labelWith("W6s", (((w - 20)/4 )+ cm) / feet) 
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 20)/ 4 )+ 9)+ cm) / feet), labelWith("WB", ((((w - 20)/ 4)+ 9)+ cm) / feet),
                        labelWith("WT", ((((w - 20)/ 4 )+ 9)+ cm) / feet), labelWith("WB", ((((w - 20)/ 4)+ 9)+ cm) / feet)
                        });

    }else if (lockType == 3)
    {
    // 🔹 Inner Sections for Handal lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1s", (((((w - 20.32)/ 2)- 5.08)/2)+ cm) / feet), labelWith("W2f", ((((w - 20.32)/ 2)+ 5.08)+ cm) / feet),
                        labelWith("W3s", (((((w - 20.32)/ 2)- 5.08)/2)+ cm) / feet), labelWith("W3s", (((((w - 20.32)/ 2)- 5.08)/ 2)+ cm) / feet),
                        labelWith("W5f", ((((w - 20.32)/ 2)+ 5.08)+ cm) / feet), labelWith("W6s", (((((w - 20.32)/ 2)- 5.08)/2)+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((((w - 20.32)/2)-5.08 )/2 )+ 5.5)+ cm)/ feet), labelWith("WB", ((((((w - 20.32)/2)-5.08 )/2 )+ 5.5)+ cm)/ feet),
                        labelWith("WT", ((((((w - 20.32)/2)-5.08 )/2 )+ 5.5)+ cm)/ feet), labelWith("WB", ((((((w - 20.32)/2)-5.08 )/2 )+ 5.5)+ cm)/ feet)
                      });
    }

    }else if (windowType == 2){

    if (lockType == 1)
    {
    // 🔹 Inner Sections for Latch lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1f", (((w - 29.5)/4 )+ cm) / feet), labelWith("W2s", (((w - 29.5)/4 )+ cm) / feet),
                        labelWith("W3s", (((w - 29.5)/4 )+ cm) / feet), labelWith("W4f", (((w - 29.5)/4 )+ cm) / feet),
                        labelWith("W5f", (((w - 29.5)/4 )+ cm) / feet), labelWith("W6s", (((w - 29.5)/4 )+ cm) / feet),
                        labelWith("W7s", (((w - 29.5)/4 )+ cm) / feet), labelWith("W8f", (((w - 29.5)/4 )+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 29.5)/4 )+ 8.5)+ cm) / feet), labelWith("WB", ((((w - 29.5)/4 )+ 8.5)+ cm) / feet),
                        labelWith("WT", ((((w - 29.5)/4 )+ 8.5)+ cm) / feet), labelWith("WB", ((((w - 29.5)/4 )+ 8.5)+ cm) / feet)
                      });
    addSection("D32", { labelWith("HL", ((h - 6.5) + cm) / feet)});
                    
    }else if (lockType == 2)
    {
    // 🔹 Inner Sections for Handal lock 
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1", (((w - 29.5)/ 4 )+ cm) / feet), labelWith("W2", (((w - 29.5)/ 4 )+ cm) / feet),
                        labelWith("W3", (((w - 29.5)/ 4 )+ cm) / feet), labelWith("W4", (((w - 29.5)/ 4 )+ cm) / feet),
                        labelWith("W5", (((w - 29.5)/ 4 )+ cm) / feet), labelWith("W6", (((w - 29.5)/ 4 )+ cm) / feet),
                        labelWith("W7", (((w - 29.5)/ 4 )+ cm) / feet), labelWith("W8", (((w - 29.5)/ 4 )+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 29.5)/ 4 )+ 5.5)+ cm) / feet), labelWith("WB", ((((w - 29.5)/ 4 )+ 5.5)+ cm) / feet),
                        labelWith("WT", ((((w - 29.5)/ 4 )+ 5.5)+ cm) / feet), labelWith("WB", ((((w - 29.5)/ 4 )+ 5.5)+ cm) / feet)
                      });

    }
}else if (windowType == 3){

   if (lockType == 1)
    {
    // 🔹 Inner Sections for Latch lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W2", (((w - 20.2)/ 3 )+ cm) / feet),
                        labelWith("W3", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W4", (((w - 20.2)/ 3 )+ cm) / feet),
                        labelWith("W5", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W6", (((w - 20.2)/ 3 )+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 20.2)/ 3 )+ 8.5)+ cm) / feet), labelWith("WB", ((((w - 20.2)/ 3 )+ 8.5)+ cm) / feet)
                        });
                    
    }else if (lockType == 2)
    {
    // 🔹 Inner Sections for Self lock 
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1s", (((w - 20.4)/ 3 )+ cm) / feet), labelWith("W2f", (((w - 20.4)/ 3 )+ cm) / feet),
                        labelWith("W3s", (((w - 20.4)/ 3 )+ cm) / feet), labelWith("W4s", (((w - 20.4)/ 3 )+ cm) / feet),
                        labelWith("W5f", (((w - 20.4)/ 3 )+ cm) / feet), labelWith("W6s", (((w - 20.4)/ 3 )+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 20.4)/ 3 )+ 9)+ cm) / feet), labelWith("WB", ((((w - 20.4)/ 3 )+ 9)+ cm) / feet),
                        labelWith("WT", ((((w - 20.4)/ 3 )+ 9)+ cm) / feet), labelWith("WB", ((((w - 20.4)/ 3 )+ 9)+ cm) / feet)
                        });

    }else if (lockType == 3)
    {
    // 🔹 Inner Sections for Handal lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet),
                        labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet)
                        });
    addSection("M24", { labelWith("W1", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W2", (((w - 20.2)/ 3 )+ cm) / feet),
                        labelWith("W3", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W4", (((w - 20.2)/ 3 )+ cm) / feet),
                        labelWith("W5", (((w - 20.2)/ 3 )+ cm) / feet), labelWith("W6", (((w - 20.2)/ 3 )+ cm) / feet)
                        });
    addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
                        labelWith("WT", ((((w - 20.2)/ 3 )+ 5.5)+ cm) / feet), labelWith("WB", ((((w - 20.2)/ 3 )+ 5.5)+ cm) / feet)
                      });
    }

}
                                   
}

double SG_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}


std::string SG_Win_f::getLabel() const {
    return label;
}

const SectionMap& SG_Win_f::getSectionMap() const {
    return sectionMap;
}


void SG_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}



double SG_Win_f::getHeight() const {  return height;}
double SG_Win_f::getWidth() const { return width;}
int SG_Win_f::getCollarType() const { return collarType; }
int SG_Win_f::getLockType() const { return lockType; }  // ✔ New getter for lock type

void SG_Win_f::sendToEstimator() {
    std::cout << "[SG_Win_f] Warning: No estimator provided in sendToEstimator().\n";
}
