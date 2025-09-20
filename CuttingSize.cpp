#include "CuttingSize.h"
#include "SettingsManagerD.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

void CuttingSize::printToFile(const EstimateLengthResult& result) {
    SettingsManagerD& settingsD = SettingsManagerD::getInstance();
    double margin = settingsD.getCuttingMargin(); // default 1.2 cm

    std::ofstream outFile("CuttingSizeReport.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: CuttingSizeReport.txt open nahi ho saki!" << std::endl;
        return;
    }

    // ek helper lambda banate hain jisse dono (console + file) me ek sath likh sake
    auto print = [&](const std::string& text) {
        std::cout << text;       // console
        outFile << text;         // file
    };

    print("================ Cutting Sizes Report ================\n\n");

    // 🔹 Iterate sections
    for (const auto& [sectionName, groups] : result.sectionGroups) {
        for (const auto& group : groups) {
            print("========== " + group.sectionName + " ==========\n");

            {
                std::ostringstream oss;
                oss << "Length : " << std::fixed << std::setprecision(1)
                    << group.stockLenFt << "ft\n";
                print(oss.str());
            }

            print("-------------------------------------------\n");
            print("window   | No#  | Dimension | Size (cm)\n");
            print("-------------------------------------------\n");

            // 🔹 Iterate cuts
            for (const auto& cut : group.cuts) {
                double sizeCm = cut.lengthFt * 30.48;  // ft → cm
                double finalCm = sizeCm - margin;

                std::ostringstream oss;
                oss << std::left << std::setw(9) << cut.windowName
                    << "| " << std::setw(4) << cut.windowNo
                    << "| " << std::setw(10) << cut.dimension
                    << "| " << std::fixed << std::setprecision(1)
                    << finalCm << "\n";
                print(oss.str());
            }

            print("-------------------------------------------\n");

            {
                std::ostringstream oss;
                oss << "WASTAGE : " << group.wastageFt << " ft\n";
                print(oss.str());
            }

            print("-------------------------------------------\n\n");
        }
    }

    outFile.close();
}
