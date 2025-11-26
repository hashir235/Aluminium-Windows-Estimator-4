#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "GroupResult.h"

struct CutInfo {
    std::string windowName;
    int windowNo;
    std::string dimension;
    double lengthFt;
};

struct OptimizedGroup {
    std::string sectionName;
    double stockLenFt;
    double wastageFt;
    bool offcut;
    std::vector<CutInfo> cuts;
};

struct SectionSummary {
    std::string sectionName;
    std::vector<double> usedLengths;
    double totalLength = 0.0;
    double totalPrice  = 0.0;
};

struct EstimateLengthResult {
    // 🔹 Raw grouped cutting data
    std::map<std::string, std::vector<OptimizedGroup>> sectionGroups;

    // 🔹 Summaries for display
    std::unordered_map<std::string, SectionSummary> sectionSummaries;
    std::unordered_map<std::string, std::vector<std::string>> groupedDisplayData;

    // --- GETTERS ---
    const std::unordered_map<std::string, SectionSummary>& getSectionSummaries() const {
        return sectionSummaries;
    }

    const std::unordered_map<std::string, std::vector<std::string>>& getGroupedSectionSummaries() const {
        return groupedDisplayData;
    }

    // --- SETTERS ---
    void setSectionSummaries(const std::unordered_map<std::string, SectionSummary>& summaries) {
        sectionSummaries = summaries;
    }

    void setGroupedSectionSummaries(const std::unordered_map<std::string, std::vector<std::string>>& displayData) {
        groupedDisplayData = displayData;
    }
};
