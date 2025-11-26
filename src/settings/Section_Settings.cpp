#include "Section_Settings.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

using namespace std;

const string Section_Settings::DEFAULT_PATH = "../data/Section_Settings.txt";

Section_Settings& Section_Settings::getInstance() {
    static Section_Settings instance;
    return instance;
}

// ----------------------------- helpers -----------------------------
string Section_Settings::trim(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

bool Section_Settings::parseIntSafe(const string& s, int &out) {
    try {
        size_t pos = 0;
        long v = stol(trim(s), &pos);
        if (pos == 0) return false;
        out = static_cast<int>(v);
        return true;
    } catch (...) {
        return false;
    }
}

// ----------------------------- Save -----------------------------
void Section_Settings::saveToFile(const std::string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "⚠ ERROR: Cannot save settings to '" << filename << "'\n";
        return;
    }

    // Section rules: NAME=14,16,18
    for (const auto& [name, vec] : sectionLengthRules) {
        out << name << "=";
        for (size_t i = 0; i < vec.size(); ++i) {
            out << vec[i];
            if (i + 1 < vec.size()) out << ",";
        }
        out << "\n";
    }

    // Key:value pairs
    for (const auto& [k, v] : settingsMap) {
        out << k << ":" << v << "\n";
    }

    out.close();
    cout << "✔ Settings saved to '" << filename << "'\n";
}

void Section_Settings::saveToFile() const {
    saveToFile(DEFAULT_PATH);
}

// ----------------------------- Load -----------------------------
#include <filesystem>   // ADD THIS

void Section_Settings::loadFromFile(const std::string& filename) {
    cout << "\n-----------------------------------------\n";
    cout << "Trying to load settings from: " << filename << "\n";
    cout << "Current Working Directory (CWD): "
         << std::filesystem::current_path().string() << "\n";

    ifstream in(filename);
    if (!in.is_open()) {
        cout << "❌ FAILED to open settings file!\n";
        cout << "→ Falling back to default settings...\n";
        loadDefaultSettings();
        cout << "-----------------------------------------\n";
        return;
    }

    sectionLengthRules.clear();
    settingsMap.clear();

    string line;
    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line[0] == '#' || line.rfind("//", 0) == 0) continue;

        // NAME=14,16,18
        size_t eq = line.find('=');
        if (eq != string::npos) {
            string key = trim(line.substr(0, eq));
            string rhs = trim(line.substr(eq + 1));

            vector<int> vals;
            string token;
            stringstream ss(rhs);

            while (getline(ss, token, ',')) {
                int v;
                if (parseIntSafe(token, v)) vals.push_back(v);
            }

            if (!vals.empty())
                sectionLengthRules[key] = vals;

            continue;
        }

        // key:value
        size_t colon = line.find(':');
        if (colon != string::npos) {
            string key = trim(line.substr(0, colon));
            string val = trim(line.substr(colon + 1));
            if (!key.empty())
                settingsMap[key] = val;
            continue;
        }
    }

    in.close();
    cout << "✅ SUCCESS — Loaded settings from '" << filename << "'\n";
    cout << "-----------------------------------------\n";
}


void Section_Settings::loadFromFile() {
    loadFromFile(DEFAULT_PATH);
}

// ------------------------- Defaults -------------------------
void Section_Settings::loadDefaultSettings() {
    sectionLengthRules = {
        {"DC26C", {14,16,18}}, {"DC26F", {15,17,19}},
        {"DC30C", {14,16,18}}, {"DC30F", {15,17,19}},
        {"M23", {14,16,18}},   {"M24", {14,16,18}},
        {"M28", {14,16,18}},   {"D29", {14,16,18}},
        {"D32A",{14,16,18}},   {"M30F",{15,17,19}},
        {"M30",{14,16,18}},    {"M26F",{15,17,19}},
        {"M26",{14,16,18}},    {"D54A",{14,16,18}},
        {"D54F",{15,17,19}},   {"D41",{14,16,18}},
        {"D50",{14,16,18}},    {"D50A",{14,16,18}},
        {"D52",{14,16,18}},    {"D46",{14,16,18}},
        {"D51A",{14,16,18}},   {"D51F",{15,17,19}}
    };

    settingsMap = {
        {"glassRate", "320"},
        {"laborRate", "150"},
        {"hardwareRate", "2200"}
    };

    cout << "✔ Default settings loaded.\n";
}

// ------------------------- API Methods -------------------------
void Section_Settings::setSectionLengths(const string& section, const vector<int>& lengths) {
    sectionLengthRules[section] = lengths;
}

vector<int> Section_Settings::getSectionLengths(const string& section) const {
    auto it = sectionLengthRules.find(section);
    if (it != sectionLengthRules.end()) return it->second;
    return {};
}

vector<double> Section_Settings::getAllowedLengthsForSection(const string& section) const {
    vector<double> out;
    auto it = sectionLengthRules.find(section);
    if (it != sectionLengthRules.end()) {
        for (int x : it->second) out.push_back(static_cast<double>(x));
    }
    return out;
}

// isGroup1: identifies if the section belongs to the 14/16/18 group
bool Section_Settings::isGroup1(const string& sectionName) const {
    auto it = sectionLengthRules.find(sectionName);
    if (it == sectionLengthRules.end()) return false;
    for (int x : it->second) {
        if (x == 14 || x == 16 || x == 18) return true;
    }
    return false;
}

void Section_Settings::renameSection(const string& oldName, const string& newName) {
    auto it = sectionLengthRules.find(oldName);
    if (it != sectionLengthRules.end()) {
        sectionLengthRules[newName] = it->second;
        sectionLengthRules.erase(it);
    }
}

void Section_Settings::setSetting(const string& key, const string& value) {
    settingsMap[key] = value;
}

string Section_Settings::getSetting(const string& key) const {
    auto it = settingsMap.find(key);
    if (it != settingsMap.end()) return it->second;
    return "";
}

string Section_Settings::getSectionName(const string& defaultName) const {
    auto it = settingsMap.find(defaultName);
    if (it != settingsMap.end()) return it->second;
    return defaultName;
}

void Section_Settings::displayAllSettings() const {
    cout << "\n==== SECTION LENGTH RULES ====\n";
    for (const auto& [sec, lengths] : sectionLengthRules) {
        cout << sec << ": ";
        for (int x : lengths) cout << x << " ";
        cout << "\n";
    }

    cout << "\n==== KEY-VALUE SETTINGS ====\n";
    for (const auto& [k, v] : settingsMap) {
        cout << k << ": " << v << "\n";
    }
}
