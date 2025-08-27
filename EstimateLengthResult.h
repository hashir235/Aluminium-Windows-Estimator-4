#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "GroupResult.h"

struct LengthGroup {
    double stockLength;
    std::vector<std::pair<std::string,double>> items;
    double wastage;
};

struct SectionSummary {
    std::string sectionName;
    std::vector<double> usedLengths; // e.g. {19, 17, 15}
    double totalLength = 0.0;   // total length (ft)
    double totalPrice  = 0.0;   // total price (ft × rate)
};


// ✅ Final structure passed from EstimateLengthManager to FinalSummaryManager
struct EstimateLengthResult {
    std::unordered_map<std::string, SectionSummary> sectionSummaries;

    // ✅ Full grouped display data (like printable view)
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
