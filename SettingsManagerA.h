#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class SettingsManagerA {
public:
    // Singleton instance
    static SettingsManagerA& getInstance();

    // File handling
    void saveToFile(const std::string& filename) const;
    void saveToFile();
    void loadFromFile(const std::string& filename);
    void loadFromFile();
    void loadSettings();  // Load default settings

    // Section rules
    void setSectionLengths(const std::string& section, const std::vector<int>& lengths);
    std::vector<int> getSectionLengths(const std::string& section) const;
    void renameSection(const std::string& oldName, const std::string& newName);

    // 🔥 NEW FUNCTION for EstimateLengthManager
    std::vector<double> getAllowedLengthsForSection(const std::string& section) const;

    // Settings map
    void setSetting(const std::string& key, const std::string& value);
    std::string getSetting(const std::string& key) const;
    std::string getSectionName(const std::string& defaultName);

    // Display
    void displayAllSettings() const;

     // 👇 NEW FUNCTION: Add this
    bool isGroup1(const std::string& sectionName);

private:
    // Private constructor for singleton
    SettingsManagerA() {}

    // Maps to hold settings
    mutable std::unordered_map<std::string, std::vector<int>> sectionLengthRules;

    std::unordered_map<std::string, std::string> settingsMap;

};

