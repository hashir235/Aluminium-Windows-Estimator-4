#include "general_settings.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

general_settings::general_settings()
    : maxExtraPieces(1), redZone1(13.0), redZone2(13.6) {} // default values

general_settings& general_settings::getInstance() {
    static general_settings instance;
    return instance;
}

bool general_settings::loadSettings(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Warning: Cannot open " << filename << ", using default values.\n";
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
                    std::cerr << " Error parsing " << key << ": " << e.what() << "\n";
                }
            }
        }
    }

    return true;
}

bool general_settings::saveSettings(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << " Error: Cannot write to " << filename << "\n";
        return false;
    }

    file << "maxExtraPieces=" << maxExtraPieces << "\n";
    file << "redZone1=" << redZone1 << "\n";
    file << "redZone2=" << redZone2 << "\n";

    return true;
}

int general_settings::getMaxExtraPieces() const {
    return maxExtraPieces;
}

double general_settings::getRedZone1() const {
    return redZone1;
}

double general_settings::getRedZone2() const {
    return redZone2;
}

void general_settings::setMaxExtraPieces(int value) {
    maxExtraPieces = value;
}

void general_settings::setRedZone1(double value) {
    redZone1 = value;
}

void general_settings::setRedZone2(double value) {
    redZone2 = value;
}
