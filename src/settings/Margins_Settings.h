#ifndef SETTINGS_MANAGER_C_H
#define SETTINGS_MANAGER_C_H

#include <string>
#include <map>
#include <iostream>

class Margins_Settings {
private:
    std::map<std::string, double> cuttingMargins; 
    std::string fileName = "../data/cutting_margins.txt";

    // Private constructor (Singleton pattern)
    Margins_Settings();

public:
    // Disable copy
    Margins_Settings(const Margins_Settings&) = delete;
    Margins_Settings& operator=(const Margins_Settings&) = delete;

    static Margins_Settings& getInstance();

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
