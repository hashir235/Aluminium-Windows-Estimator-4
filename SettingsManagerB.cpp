#include "SettingsManagerB.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

SettingsManagerB::SettingsManagerB()
    : maxExtraPieces(1), redZone1(13.0), redZone2(13.6) {} // default values

SettingsManagerB& SettingsManagerB::getInstance() {
    static SettingsManagerB instance;
    return instance;
}

bool SettingsManagerB::loadSettings(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "⚠️ Warning: Cannot open " << filename << ", using default values.\n";
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (getline(iss, key, '=')) {
            std::string valueStr;
            if (getline(iss, valueStr)) {
                try {
                    if (key == "maxExtraPieces") {
                        maxExtraPieces = std::stoi(valueStr);
                    } else if (key == "redZone1") {
                        redZone1 = std::stod(valueStr);
                    } else if (key == "redZone2") {
                        redZone2 = std::stod(valueStr);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "⚠️ Error parsing " << key << ": " << e.what() << "\n";
                }
            }
        }
    }

    return true;
}

bool SettingsManagerB::saveSettings(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "❌ Error: Cannot write to " << filename << "\n";
        return false;
    }

    file << "maxExtraPieces=" << maxExtraPieces << "\n";
    file << "redZone1=" << redZone1 << "\n";
    file << "redZone2=" << redZone2 << "\n";

    return true;
}

int SettingsManagerB::getMaxExtraPieces() const {
    return maxExtraPieces;
}

double SettingsManagerB::getRedZone1() const {
    return redZone1;
}

double SettingsManagerB::getRedZone2() const {
    return redZone2;
}

void SettingsManagerB::setMaxExtraPieces(int value) {
    maxExtraPieces = value;
}

void SettingsManagerB::setRedZone1(double value) {
    redZone1 = value;
}

void SettingsManagerB::setRedZone2(double value) {
    redZone2 = value;
}
