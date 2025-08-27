#include "SettingsManagerA.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

SettingsManagerA& SettingsManagerA::getInstance() {
    static SettingsManagerA instance;
    return instance;
}

// ===== SAVE SETTINGS TO FILE =====

void SettingsManagerA::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "❌ Error: Cannot open file for saving settings.\n";
        return;
    }

    // Save all section length rules
    for (const auto& [section, lengths] : sectionLengthRules) {
        outFile << section << "=";
        for (size_t i = 0; i < lengths.size(); ++i) {
            outFile << lengths[i];
            if (i < lengths.size() - 1) outFile << ",";
        }
        outFile << "\n";
    }

    // Save general settings
    for (const auto& [key, value] : settingsMap) {
        outFile << key << ":" << value << "\n";
    }

    std::cout << "✅ Settings saved to '" << filename << "'\n";
}


void SettingsManagerA::saveToFile() {
    saveToFile("section_settings.txt");
}

// ===== LOAD SETTINGS FROM FILE =====

void SettingsManagerA::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cout << "⚠️ Settings file not found. Using default settings.\n";
        return;
    }

    sectionLengthRules.clear();
    settingsMap.clear();

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.find('=') != std::string::npos) {
            std::string key = line.substr(0, line.find('='));
            std::string valueStr = line.substr(line.find('=') + 1);
            std::stringstream ss(valueStr);
            std::string token;
            std::vector<int> lengths;

            while (std::getline(ss, token, ',')) {
                lengths.push_back(std::stoi(token));
            }

            sectionLengthRules[key] = lengths;
        } else if (line.find(':') != std::string::npos) {
            std::string key = line.substr(0, line.find(':'));
            std::string value = line.substr(line.find(':') + 1);
            settingsMap[key] = value;
        }
    }

    std::cout << "📥 Loaded settings from file '" << filename << "'\n";
}

void SettingsManagerA::loadFromFile() {
    loadFromFile("section_settings.txt");
}

// ===== SECTION LENGTH METHODS =====

void SettingsManagerA::setSectionLengths(const std::string& section, const std::vector<int>& lengths) {
    sectionLengthRules[section] = lengths;
}

std::vector<int> SettingsManagerA::getSectionLengths(const std::string& section) const {
    auto it = sectionLengthRules.find(section);
    if (it != sectionLengthRules.end()) {
        return it->second;
    }
    return {};
}

void SettingsManagerA::renameSection(const std::string& oldName, const std::string& newName) {
    auto it = sectionLengthRules.find(oldName);
    if (it != sectionLengthRules.end()) {
        sectionLengthRules[newName] = it->second;
        sectionLengthRules.erase(it);
    }
}

void SettingsManagerA::displayAllSettings() const {
    std::cout << "\n📋 All Section Length Rules:\n";
    for (const auto& [section, lengths] : sectionLengthRules) {
        std::cout << "• " << section << ": ";
        for (int l : lengths) std::cout << l << " ";
        std::cout << "\n";
    }

    std::cout << "\n⚙️ Key-Value Settings:\n";
    for (const auto& [key, val] : settingsMap) {
        std::cout << "• " << key << ": " << val << "\n";
    }
}

// ===== OTHER SETTINGS METHODS =====

void SettingsManagerA::setSetting(const std::string& key, const std::string& value) {
    settingsMap[key] = value;
}

std::string SettingsManagerA::getSectionName(const std::string& defaultName) {
    auto it = settingsMap.find(defaultName);
    if (it != settingsMap.end()) {
        return it->second;
    }
    return defaultName;
}

std::string SettingsManagerA::getSetting(const std::string& key) const {
    auto it = settingsMap.find(key);
    if (it != settingsMap.end()) return it->second;
    return "";
}

// ===== DEFAULT SETTINGS LOADER =====

void SettingsManagerA::loadSettings() {
    sectionLengthRules = {
        {"DC30F", {15, 17, 19}},
        {"DC26F", {15, 17, 19}},
        {"DC30C", {14, 16, 18}},
        {"DC26C", {14, 16, 18}},
        {"M23", {14, 16, 18}},
        {"M24", {14, 16, 18}},
        {"M28", {14, 16, 18}},
        {"D29", {14, 16, 18}}
    };

    settingsMap = {
        {"glassRate", "320"},
        {"laborRate", "150"},
        {"hardwareRate", "2200"}
    };

    std::cout << "🔁 Default settings loaded.\n";
}

std::vector<double> SettingsManagerA::getAllowedLengthsForSection(const std::string& section) const {
    std::vector<double> result;
    auto it = sectionLengthRules.find(section);
    if (it != sectionLengthRules.end()) {
        for (int val : it->second) {
            result.push_back(static_cast<float>(val));  // Convert int todouble
        }
    }
    return result;
}

bool SettingsManagerA::isGroup1(const std::string& sectionName) {
    std::vector<double> allowed = getAllowedLengthsForSection(sectionName);
    // Group 1: 14, 16, 18
    // Group 2: 15, 17, 19

    // agar first length group 1 me hai to isGroup1 true
    if (!allowed.empty() && (allowed[0] == 14 || allowed[0] == 16 || allowed[0] == 18)) {
        return true;
    }
    return false;
}

