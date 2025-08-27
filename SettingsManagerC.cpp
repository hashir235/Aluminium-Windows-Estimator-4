#include "SettingsManagerC.h"
#include <fstream>
#include <sstream>

SettingsManagerC::SettingsManagerC() {
    loadFromFile();
}

SettingsManagerC& SettingsManagerC::getInstance() {
    static SettingsManagerC instance;
    return instance;
}

void SettingsManagerC::loadFromFile() {
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

void SettingsManagerC::saveToFile() const {
    std::ofstream file(fileName);
    if (!file) {
        std::cerr << "❌ Could not save to " << fileName << "\n";
        return;
    }
    for (const auto& kv : cuttingMargins) {
        file << kv.first << "=" << kv.second << "\n";
    }
    file.close();
}

double SettingsManagerC::getMargin(const std::string& section) const {
    auto it = cuttingMargins.find(section);
    if (it != cuttingMargins.end()) {
        return it->second;
    }
    return 0.0; // default margin
}

void SettingsManagerC::setMargin(const std::string& section, double margin) {
    cuttingMargins[section] = margin;
}

void SettingsManagerC::showAllMargins() const {
    std::cout << "\n--- Cutting Margins ---\n";
    for (const auto& kv : cuttingMargins) {
        std::cout << kv.first << " = " << kv.second << " ft\n";
    }
}
const std::map<std::string, double>& SettingsManagerC::getAllMargins() const {
    return cuttingMargins;
}
