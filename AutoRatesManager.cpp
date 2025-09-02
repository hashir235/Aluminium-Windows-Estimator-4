#include "AutoRatesManager.h"  // Isme hi windows macros + <windows.h> handle ho gaya

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <curl/curl.h>


// ---- Helper for libcurl ----
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

AutoRatesManager::AutoRatesManager() {
    cout << "🔄 Checking for latest rate list..." << endl;

    // 1. Try online update
    if (!updateRateListFromServer(ratesUrl)) {
        cout << "⚠️ Could not fetch online rates.\n";

        // 2. Try local CSV
        if (!loadRatesFromCSV(localRatesFile)) {
            cout << "❌ No local rate file found!\n";
            cout << "👉 Please connect to internet or use manual entry option.\n";
        } else {
            cout << "📂 Loaded last saved rates from local file.\n";
        }
    }
}

bool AutoRatesManager::checkAndUpdateRates() {
    std::cout << "🔄 Checking and updating rates...\n";

    bool updateSuccess = updateRateListFromServer(ratesUrl);

    if (updateSuccess) {
        std::cout << "\n-------------------------------------------------\n";
        std::cout << "✅ Rates updated successfully from server.\n";
        std::cout << "-------------------------------------------------\n";
    } else {
        std::cout << "\n-------------------------------------------------\n";
        std::cout << "⚠️ Could not update rates (offline mode, using last saved rates).\n";
        std::cout << "-------------------------------------------------\n";
    }

    return updateSuccess;
}


// Download + Save file
bool AutoRatesManager::updateRateListFromServer(const string& url) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return false;
        }

        // Save to local file
        ofstream outFile(localRatesFile);
        if (!outFile.is_open()) return false;
        outFile << readBuffer;
        outFile.close();

        // Load into memory
        loadRatesFromCSV(localRatesFile);
        cout << "✅ Rate list updated successfully!\n";
        return true;
    }
    return false;
}


bool AutoRatesManager::loadRatesFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "⚠️ Could not open " << filename << "!\n";
        return false;
    }

    sectionRates.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string section;
        double rate;
        if (getline(ss, section, ',') && ss >> rate) {
            sectionRates[section] = rate;
        }
    }
    file.close();

    if (sectionRates.empty()) {
        cout << "⚠️ CSV file empty/corrupt!\n";
        return false;
    }

    return true;   // ✅ successfully load ho gaya
}




void AutoRatesManager::printFinalSummaryTable() const {
    cout << "\n===== Final Section Rates =====\n";
    for (const auto& entry : sectionRates) {
        cout << setw(10) << entry.first << " : " << entry.second << "\n";
    }
}

// ----------------- Helper Functions -----------------
static inline string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static string normalizeSectionName(const string &s) {
    string out;
    bool space = false;
    for (char c : s) {
        if (isspace((unsigned char)c)) {
            if (!space) {
                out.push_back(' ');
                space = true;
            }
        } else {
            out.push_back(c);
            space = false;
        }
    }
    return trim(out);
}

// ----------------- Main Functions -----------------
void AutoRatesManager::selectOptions() {
    cout << "\nSelect Aluminium Thickness:\n";
    cout << "[1] 1.2mm\n[2] 1.6mm\n[3] 2mm\n";
    cin >> thicknessChoice;

    cout << "\nSelect Aluminium Color:\n";
    cout << "[1] DULL\n[2] H23/PC-RAL\n[3] SAHRA/BRN\n[4] BLACK/MULTI\n[5] WOOD COAT\n";
    cin >> colorChoice;

    // ❌ No default rates here
}

//------------------ Load Default Rates massage -----------------
void AutoRatesManager::loadDefaultRates() {
    // No default rates anymore
    cout << "⚠️ No default rates available. Please use MANUAL option." << endl;
}

double AutoRatesManager::getRate(const string& section) const{
    auto it = sectionRates.find(section);
    if (it != sectionRates.end()) {
        return it->second;
    }
    return 0.0;
}


void AutoRatesManager::showRatesTable() {
    cout << "\n≡ Enter Rates per ft for Each Section:\n";
    int line = 1;
    vector<string> keys;

    for (auto& kv : sectionRates) {
        cout << line << ". Rate for section [" << kv.first << "] = Rs. " << kv.second << "\n";
        keys.push_back(kv.first);
        line++;
    }

    while (true) {
        cout << "\nEnter line number to correct a rate (0 = done): ";
        int choice; cin >> choice;
        if (choice == 0) break;
        if (choice > 0 && choice <= (int)keys.size()) {
            string sec = keys[choice - 1];
            cout << "Enter new rate for [" << sec << "] = Rs. ";
            cin >> sectionRates[sec];
            cout << "✅ Rate updated.\n";
        }
    }
}

double AutoRatesManager::computeAndStoreTotals(const map<string, double>& sectionUsage) {
    cout << "\n≡ Final Aluminum Section Cost Table\n";
    cout << "-------------------------------------------------------------\n";
    cout << "Section    | Total ft  | Rate     | Total Price\n";
    cout << "-------------------------------------------------------------\n";

    double grandTotal = 0.0;
    for (auto& kv : sectionUsage) {
        string sec = kv.first;
        double length = kv.second;
        double rate = (sectionRates.count(sec) ? sectionRates[sec] : 0.0);
        double price = length * rate;
        cout << sec << "    | " << length << " | Rs. " << rate << " | Rs. " << price << "\n";
        grandTotal += price;
    }

    cout << "-------------------------------------------------------------\n";
    cout << "Grand Total Aluminum Cost = Rs. " << grandTotal << "\n";

    return grandTotal;
}
