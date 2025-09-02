// ----- Windows specific macros & includes -----
#define WIN32_LEAN_AND_MEAN   // Windows headers ko slim banata hai
#ifndef NOMINMAX
#define NOMINMAX              // Avoid min/max macro conflicts
#endif
#define byte win_byte_override // Windows ke "byte" ko rename karo

#include <windows.h>
#undef byte   // taake std::byte safe rahe

// ----- Header guard -----
#ifndef AUTORATESMANAGER_H
#define AUTORATESMANAGER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class AutoRatesManager {
private:
    map<string, double> sectionRates;   // section -> rate (per object)
    int thicknessChoice;                // per object
    int colorChoice;                    // per object

    const std::string ratesUrl = "https://raw.githubusercontent.com/hashir235/Al_Ratelist/main/RateList.csv";
    const std::string localRatesFile = "RateList.csv";

    bool downloadFile(const std::string& url, const std::string& outputFile);

public:
    AutoRatesManager();
    bool checkAndUpdateRates();

    void selectOptions();   // thickness + color
    void showRatesTable();  // show + allow editing
    double computeAndStoreTotals(const map<string, double>& sectionUsage);

    // 🔥 New functions jo tum main.cpp me call kar rahe ho
    bool loadRatesFromCSV(const std::string& filename);
    void printFinalSummaryTable() const;
    bool updateRateListFromServer(const std::string& url);
    double getRate(const std::string& sectionName) const;

private:
    void loadDefaultRates();   // ab static nahi, normal member function
};

#endif // AUTORATESMANAGER_H
