#include "SettingsManagerD.h"
#include <fstream>
#include <sstream>
#include <iostream>

SettingsManagerD::SettingsManagerD() {
    settingsFile = "fabricator_settings.txt";
    loadSettings();
}

SettingsManagerD& SettingsManagerD::getInstance() {
    static SettingsManagerD instance;
    return instance;
}

void SettingsManagerD::loadSettings() {
    std::ifstream inFile(settingsFile);
    if (!inFile) return;

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        settings[key] = value;
    }
}

void SettingsManagerD::saveSettings() {
    std::ofstream outFile(settingsFile);
    if (!outFile) {
        std::cerr << "Error saving fabricator settings!\n";
        return;
    }
    for (const auto& pair : settings) {
        outFile << pair.first << "=" << pair.second << "\n";
    }
}

double SettingsManagerD::getCuttingMargin(double defaultValue) const {
    auto it = settings.find("cuttingMargin");
    if (it != settings.end()) {
        try {
            return std::stod(it->second);
        } catch (...) {
            return defaultValue; // agar invalid value ho
        }
    }
    return defaultValue;
}

void SettingsManagerD::setCuttingMargin(double value) {
    settings["cuttingMargin"] = std::to_string(value);
}
