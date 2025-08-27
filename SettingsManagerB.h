#pragma once
#include <string>

class SettingsManagerB {
public:
    static SettingsManagerB& getInstance();

    bool loadSettings(const std::string& filename = "general_settings.txt");
    bool saveSettings(const std::string& filename = "general_settings.txt");

    int getMaxExtraPieces() const;
    double getRedZone1() const;
    double getRedZone2() const;

    void setMaxExtraPieces(int value);
    void setRedZone1(double value);
    void setRedZone2(double value);

private:
    SettingsManagerB(); // private constructor
    int maxExtraPieces;
    double redZone1;
    double redZone2;
};
