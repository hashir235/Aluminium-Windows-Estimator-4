// AutoRatesManager.h
#ifndef AUTORATESMANAGER_H
#define AUTORATESMANAGER_H

// ----- Windows specific macros & includes (kept same style as your repo) -----
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #define byte win_byte_override
    #include <windows.h>
    #undef byte
#endif

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <istream>
#include "EstimateLengthResult.h" // for SectionSummary

using namespace std;

class AutoRatesManager {
private:
    // section -> rates for each color (in the same order as colorHeaders)
    map<string, vector<double>> sectionRates;

    // header color names (order preserved)
    vector<string> colorHeaders;

    // user choices (1-based)
    int thicknessChoice = 1; // 1 => 1.2mm, 2 => 1.6mm, 3 => 2mm
    int colorChoice = 1;     // 1..N correspond to header columns

    // remote CSV file
    const string ratesUrl = "https://raw.githubusercontent.com/hashir235/Al_Ratelist/main/RateList.csv";

    // helpers
    bool parseRatesFromCSV(istream& input);
    static string trim(const string &s);
    static string normalizeKey(const string &s);
    static string gaugeStringForChoice(int choice);
    int colorIndex() const; // zero-based index from colorChoice

    bool autoRatesEnabled = false; // track karega mode ko

public:
    AutoRatesManager();

    // Update helpers
    bool checkAndUpdateRates();
    bool updateRateListFromServer(const string& url); // uses libcurl
    bool loadRatesFromCSV(const string& filename);    // local fallback

    // UI
    void selectOptions();    // ask user for thickness & color
    void showLoadedRates() const;            // prints loaded rates (all rows)
    void reviewAndEditRates(std::vector<SectionSummary>& sections);
    void printFinalSummaryTable() const;     // prints loaded rate map (debug)

    // Main computation: update SectionSummary.totalPrice and return grand total
    double computeAndStoreTotals(vector<SectionSummary>& sections);

    // Get single rate for a section using currently selected gauge/color
    double getRateForSection(const string& sectionName) const;

    // direct getter (debug)
    const map<string, vector<double>>& getAllRates() const { return sectionRates; }
    const vector<string>& getColorHeaders() const { return colorHeaders; }

     // Setter for thickness
    void setThicknessChoice(int t) {
        if (t < 1 || t > 3) t = 1; // validation
        thicknessChoice = t;
    }

    // Setter for color
    void setColorChoice(int c) {
        if (c < 1) c = 1;
        if (c > 5) c = 5; // assuming 5 colors
        colorChoice = c;
    }

    // Already existing getters (or add if missing)
    int getThicknessChoice() const { return thicknessChoice; }
    int getColorChoice() const { return colorChoice; }
    void enableAutoRates(bool enabled) { autoRatesEnabled = enabled; }
    bool isAutoRatesEnabled() const { return autoRatesEnabled; }

    // ⬇️ Add this new function declaration
    void generateMaterialTxt(const std::vector<SectionSummary>& sections);

    void reset(); // clear all loaded data and reset choices
};

#endif // AUTORATESMANAGER_H
