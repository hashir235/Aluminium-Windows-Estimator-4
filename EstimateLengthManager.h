#pragma once

#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "GroupResult.h"
#include "CommonTypes.h"
#include "EstimateLengthResult.h"

class EstimateLengthManager {
private:
    std::unordered_map<std::string, SectionMap> windowSections;
    std::unordered_map<std::string, std::vector<std::pair<double, std::string>>> allSections;
    std::unordered_map<std::string, std::vector<std::string>> groupResults;
    std::map<std::string, SectionSummary> optimizedSectionSummary;
    std::unordered_map<std::string, std::vector<std::vector<std::pair<std::string, double>>>> sectionSizes;
    EstimateLengthResult groupedSectionSummaries;
    mutable std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> combinedSections;

    // ✅ Stores per-section summaries
    std::vector<SectionSummary> sectionSummaries;

    std::unordered_map<std::string,double> sectionRates;

public:
    EstimateLengthManager();

    void resetSectionRates();
    void addWindowSections(const std::string& label, const SectionMap& sectionMap);
    void printPerSectionBreakdown() const;
    EstimateLengthResult groupAndOptimizeSections();

    std::pair<std::vector<std::vector<std::pair<double, std::string>>>, std::vector<std::pair<double, std::string>>> groupLengthsGreedy(
        const std::vector<std::pair<double, std::string>>& pieces,
        const std::vector<double>& allowedStock
    );

    // Reset all stored data
    void clearAllData();

    void showFinalReport();
    void performEstimation();
    void addSectionSizes(const std::string& section, const std::vector<std::pair<std::string, double>>& sizes);

    const std::map<std::string, SectionSummary>& getOptimizedSummary() const;
    const std::unordered_map<std::string, std::vector<std::string>>& getGroupResults() const;

    // ✅ Only ONE declaration of this function
    const EstimateLengthResult& getGroupedSectionSummaries() const;
    void setGroupedSectionSummaries(const EstimateLengthResult& result);

    void removeWindow(const std::string& label);

    // ✅ Accessor for section summaries
    const std::vector<SectionSummary>& getSummaries() const;
    std::vector<SectionSummary>& getSummaries();

    bool isEmpty() const {
        return sectionSummaries.empty();
    }

    bool hasOptimizedData() const;
};
