#include "SM_Win_f.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerC.h"
#include "SettingsManagerD.h"
#include "CommonTypes.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>


int SM_Win_f::windowCount = 0; 
void SM_Win_f::resetWindowCount() { windowCount = 0;}
SM_Win_f::SM_Win_f() {
    height = 0.0;
    width = 0.0;
    collarType = 0;
    lockType = 0;
    ++windowCount;
}


bool SM_Win_f::inputWindowData(bool isEditing) {
    if (!isEditing) {  
        label = "SM_Win_f #" + std::to_string(windowCount) + " -> ";
    } else {
        std::cout << "\n--- Editing " << label << " ---\n";
    }

    // 🔹 Lock type selection (First)
    int lockChoice = 0;
    while (true) {
        std::cout << "Select Lock Type:\n";
        std::cout << "[1] Latch lock\n";
        std::cout << "[2] Self lock\n";
        std::cout << "[3] Handle/Push/Button lock\n";
        std::cout << "Enter choice (1-3, 0 = cancel): ";

        if (std::cin >> lockChoice) {
            if (lockChoice == 0) {
                std::cout << "⚠ Entry cancelled!\n";
                return false;
            }
            if (lockChoice >= 1 && lockChoice <= 3) {
                lockType = lockChoice; // save lock type
                break;
            }
        }
        std::cout << "❌ Invalid choice! Please enter 1, 2, or 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 🔹 Collar type
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

    // 🔹 Height (cm)
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

    // 🔹 Width (cm)
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




void SM_Win_f::calculateSections() {
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
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 2:
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 3:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("M30", { labelWith("WT", ((w) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 4:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M30", { labelWith("HR", ((h) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) }); 
            break;
        case 5:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 6:
            addSection("M30F", { labelWith("HR", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M30", { labelWith("HL", ((h) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 7:
            addSection("M30F", { labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 8:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M30", { labelWith("HR", ((h) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 9:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet), labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("M30", { labelWith("WT", ((w) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 10:
            addSection("M30F", { labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 11:
            addSection("M30F", { labelWith("WT", ((w + 6) + cm)/feet) });
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 12:
            addSection("M30F", { labelWith("HR", ((h + 6) + cm)/feet) });
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
        case 13:
            addSection("M30", { labelWith("HL", ((h) + cm)/feet), labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("M26F", { labelWith("WB", ((w + 6) + cm)/feet) });
            break;
        case 14:
            addSection("M30F", { labelWith("HL", ((h + 6) + cm)/feet) });
            addSection("M30", { labelWith("HR", ((h) + cm)/feet), labelWith("WT", ((w) + cm)/feet) });
            addSection("M26", { labelWith("WB", ((w) + cm)/feet) });
            break;
    }

    if (lockType == 1)
    {
    // 🔹 Inner Sections for Latch lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M24", { labelWith("W1", (((w - 15.5)/2 )+ cm) / feet), labelWith("W2", (((w - 15.5)/2 )+ cm) / feet),
                        labelWith("W3", (((w - 15.5)/2 )+ cm) / feet), labelWith("W4", (((w - 15.5)/2 )+ cm) / feet)
                      });
    // addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
    //                     labelWith("WT", ((((w - 15.5)/2 )+8.5)+ cm) / feet), labelWith("WB", ((((w - 15.5)/2 )+8.5)+ cm) / feet)
    //                   });
                    
    }else if (lockType == 2)
    {
    // 🔹 Inner Sections for Self lock 
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M24", { labelWith("W1", (((w - 17.2)/2 )+ cm) / feet), labelWith("W2", (((w - 17.2)/2 )+ cm) / feet),
                        labelWith("W3", (((w - 17.2)/2 )+ cm) / feet), labelWith("W4", (((w - 17.2)/2 )+ cm) / feet)
                      });
    // addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
    //                     labelWith("WT", ((((w - 17.2)/2 )+ 9)+ cm) / feet), labelWith("WB", ((((w - 17.2)/2 )+ 9)+ cm) / feet)
    //                   });  

    }else if (lockType == 3)
    {
    // 🔹 Inner Sections for Handle/Push/Button lock
    addSection("M23", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M28", { labelWith("H", ((h - 4.2) + cm) / feet), labelWith("H", ((h - 4.2) + cm) / feet) });
    addSection("M24", { labelWith("W1", (((w - 15.5)/2 )+ cm) / feet), labelWith("W2", (((w - 15.5)/2 )+ cm) / feet),
                        labelWith("W3", (((w - 15.5)/2 )+ cm) / feet), labelWith("W4", (((w - 15.5)/2 )+ cm) / feet)
                      });
    // addSection("D29", { labelWith("HL", ((h - 4.2) + cm) / feet), labelWith("HR", ((h - 4.2) + cm) / feet),
    //                     labelWith("WT", ((((w - 15.5)/2 )+ 5.5)+ cm) / feet), labelWith("WB", ((((w - 15.5)/2 )+ 5.5)+ cm) / feet)
    //                   });
    }
                                   
}


std::string SM_Win_f::getLabel() const {
    return label;
}

const SectionMap& SM_Win_f::getSectionMap() const {
    return sectionMap;
}


void SM_Win_f::sendToEstimator(EstimateLengthManager& estimator) {
    estimator.addWindowSections(label, sectionMap);
}


//char SM_Win_f::getSize() const { return unitChoice; }

double SM_Win_f::getAreaSqFt() const {
    return 0.0;  // Not applicable for F-series
}

double SM_Win_f::getHeight() const {  return height;}
double SM_Win_f::getWidth() const { return width;}
int SM_Win_f::getCollarType() const { return collarType; }
int SM_Win_f::getLockType() const { return lockType; }  // ✔ New getter for lock type

void SM_Win_f::sendToEstimator() {
    std::cout << "[SM_Win_f] Warning: No estimator provided in sendToEstimator().\n";
}
