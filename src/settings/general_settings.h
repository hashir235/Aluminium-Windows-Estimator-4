#pragma once
#include <string>

class general_settings {
public:
    static general_settings& getInstance();

    bool loadSettings(const std::string& filename = "../data/general_settings.txt");
    bool saveSettings(const std::string& filename = "../data/general_settings.txt");

    int getMaxExtraPieces() const;
    double getRedZone1() const;
    double getRedZone2() const;

    void setMaxExtraPieces(int value);
    void setRedZone1(double value);
    void setRedZone2(double value);

private:
    general_settings(); // private constructor
    int maxExtraPieces;
    double redZone1;
    double redZone2;
};
