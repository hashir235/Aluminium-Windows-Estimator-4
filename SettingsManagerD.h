#ifndef SETTINGS_MANAGER_D_H
#define SETTINGS_MANAGER_D_H

#include <string>
#include <unordered_map>

class SettingsManagerD {
private:
    std::unordered_map<std::string, std::string> settings;
    std::string settingsFile;

    SettingsManagerD(); // singleton
    void loadSettings();

public:
    static SettingsManagerD& getInstance();

    double getCuttingMargin(double defaultValue = 1.2) const;
    void setCuttingMargin(double value);

    void saveSettings();
};

#endif
