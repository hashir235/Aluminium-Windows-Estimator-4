#pragma once

#include <string>
#include <vector>

struct GroupResult {
    double stockLengthUsed;
    std::vector<std::pair<std::string, double>> itemsUsed;
    double wastage;

    GroupResult(double length, const std::vector<std::pair<std::string, double>>& items, double waste)
        : stockLengthUsed(length), itemsUsed(items), wastage(waste) {}

    GroupResult() : stockLengthUsed(0.0), wastage(0.0) {}
};
