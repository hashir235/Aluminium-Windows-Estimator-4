#ifndef SETTINGS_MANAGER_C_H
#define SETTINGS_MANAGER_C_H

#include <string>
#include <map>
#include <iostream>

class SettingsManagerC {
private:
    std::map<std::string, double> cuttingMargins; 
    std::string fileName = "cutting_margins.txt";

    // Private constructor (Singleton pattern)
    SettingsManagerC();

public:
    // Disable copy
    SettingsManagerC(const SettingsManagerC&) = delete;
    SettingsManagerC& operator=(const SettingsManagerC&) = delete;

    static SettingsManagerC& getInstance();

    // Load & Save
    void loadFromFile();
    void saveToFile() const;

    // Access margins
    double getMargin(const std::string& section) const;
    void setMargin(const std::string& section, double margin);

    // For debugging / menu
    void showAllMargins() const;

    // return full map of section->margin
    const std::map<std::string, double>& getAllMargins() const;

};

#endif
