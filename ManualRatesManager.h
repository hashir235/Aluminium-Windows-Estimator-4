#ifndef SECTION_RATES_MANAGER_H
#define SECTION_RATES_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "EstimateLengthResult.h" // for SectionSummary

class ManualRatesManager {
public:
    explicit ManualRatesManager(std::vector<SectionSummary>& summaries);

    void inputRatesFromUser();          // ask rates line-by-line
    void editRateByLineNumber();        // fix a specific line (0 = skip)
    double computeAndStoreTotals();     // sets summary.totalPrice & returns grand total
    void printFinalSummaryTable() const;
    double getTotalAluminiumCost() const; // returns grand total aluminium cost

    const std::map<std::string,double>& getRates() const { return sectionRates; }

private:
    std::map<std::string,double> sectionRates;      // sectionName -> rate
    std::vector<SectionSummary>& sections; 
     double mutable grand = 0.0;         // reference to estimator's summaries
};

#endif // SECTION_RATES_MANAGER_H
