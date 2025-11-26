#include "Margins_Settings.h"
#include <fstream>
#include <sstream>

Margins_Settings::Margins_Settings() {
    loadFromFile();
}

Margins_Settings& Margins_Settings::getInstance() {
    static Margins_Settings instance;
    return instance;
}

void Margins_Settings::loadFromFile() {
    cuttingMargins.clear();
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "⚠ Could not open " << fileName << ", using defaults.\n";
        // Default margins
        cuttingMargins["DC30F"] = 0.25;
        cuttingMargins["DC26F"] = 0.25;
        cuttingMargins["DC30C"] = 0.0410;
        cuttingMargins["DC26C"] = 0.0410;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // skip comments
        std::istringstream iss(line);
        std::string key;
        double value;
        if (std::getline(iss, key, '=') && (iss >> value)) {
            cuttingMargins[key] = value;
        }
    }
    file.close();
}

void Margins_Settings::saveToFile() const {
    std::ofstream file(fileName);
    if (!file) {
        std::cerr << " Could not save to " << fileName << "\n";
        return;
    }
    for (const auto& kv : cuttingMargins) {
        file << kv.first << "=" << kv.second << "\n";
    }
    file.close();
}

double Margins_Settings::getMargin(const std::string& section) const {
    auto it = cuttingMargins.find(section);
    if (it != cuttingMargins.end()) {
        return it->second;
    }
    return 0.0; // default margin
}

void Margins_Settings::setMargin(const std::string& section, double margin) {
    cuttingMargins[section] = margin;
}

void Margins_Settings::showAllMargins() const {
    std::cout << "\n--- Cutting Margins ---\n";
    for (const auto& kv : cuttingMargins) {
        std::cout << kv.first << " = " << kv.second << " ft\n";
    }
}
const std::map<std::string, double>& Margins_Settings::getAllMargins() const {
    return cuttingMargins;
}
