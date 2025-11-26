#ifndef SETTINGS_MANAGER_D_H
#define SETTINGS_MANAGER_D_H

#include <string>
#include <unordered_map>

class fabricator_settings {
private:
    std::unordered_map<std::string, std::string> settings;
    std::string settingsFile;

    fabricator_settings(); // singleton
    void loadSettings();

public:
    static fabricator_settings& getInstance();

    double getCuttingMargin(double defaultValue = 1.2) const;
    void setCuttingMargin(double value);

    void saveSettings();
};

#endif
