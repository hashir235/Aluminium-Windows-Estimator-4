#include "CuttingSize.h"
#include "fabricator_settings.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

void CuttingSize::printToFile(const EstimateLengthResult& result , int mode) {
    fabricator_settings& settingsD = fabricator_settings::getInstance();
    double margin = settingsD.getCuttingMargin(); // default 1.2 cm

    std::ofstream outFile("data/CuttingSizeReport.txt", std::ios::out | std::ios::trunc);

    if (!outFile.is_open()) {
        std::cerr << "Error: CuttingSizeReport.txt open nahi ho saki!" << std::endl;
        return;
    }

    // ek helper lambda banate hain jisse dono (console + file) me ek sath likh sake
    auto print = [&](const std::string& text) {
        std::cout << text;       // console
        outFile << text;         // file
    };

    // 🔹 MODE ke mutabiq heading choose karo
    std::string heading;
    if (mode == 1) {
        heading = "================ Estimate Cutting Size (ft) Report ================\n\n";
    } 
    else if (mode == 2) {
        heading = "================ Fabricate Cutting Size (cm) Report ================\n\n";
    } 
    else {
        heading = "================ Cutting Sizes Report ================\n\n"; // fallback
    }

    print(heading);

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

            std::ostringstream oss;

            double printedValue = 0.0;

            if (mode == 1) {
                // ESTIMATOR → output in ft (NO cm conversion)
                printedValue = cut.lengthFt;  
                oss << std::left << std::setw(9) << cut.windowName
                << "| " << std::setw(4) << cut.windowNo
                << "| " << std::setw(10) << cut.dimension
                << "| " << std::fixed << std::setprecision(2)
                << printedValue << " (ft)\n";
            }
            else if (mode == 2) {
                // FABRICATOR → output in cm (convert ft → cm)
                double sizeCm = cut.lengthFt * 30.48;  // ft → cm
                double finalCm = sizeCm - margin;      // apply cutting margin
                printedValue = finalCm;

            oss << std::left << std::setw(9) << cut.windowName
                << "| " << std::setw(4) << cut.windowNo
                << "| " << std::setw(10) << cut.dimension
                << "| " << std::fixed << std::setprecision(1)
                << printedValue << "\n";   // in cm
            }

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
