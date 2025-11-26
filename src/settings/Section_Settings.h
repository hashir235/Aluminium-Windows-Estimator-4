#ifndef SECTION_SETTINGS_H
#define SECTION_SETTINGS_H

#include <string>
#include <vector>
#include <map>

class Section_Settings {
public:
    // Singleton accessor
    static Section_Settings& getInstance();

    // Load / Save (file path overloads)
    void loadFromFile(const std::string& filename);
    void loadFromFile();                        // uses DEFAULT_PATH
    void saveToFile(const std::string& filename) const;
    void saveToFile() const;                    // uses DEFAULT_PATH

    // Load built-in defaults (used if file missing or on demand)
    void loadDefaultSettings();

    // API: section length rules
    void setSectionLengths(const std::string& section, const std::vector<int>& lengths);
    std::vector<int> getSectionLengths(const std::string& section) const;
    std::vector<double> getAllowedLengthsForSection(const std::string& section) const;

    // Helpers for grouping logic (used by EstimateLengthManager)
    bool isGroup1(const std::string& sectionName) const;

    // Other utilities
    void renameSection(const std::string& oldName, const std::string& newName);
    void setSetting(const std::string& key, const std::string& value);
    std::string getSetting(const std::string& key) const;
    std::string getSectionName(const std::string& defaultName) const;

    // Debug / display
    void displayAllSettings() const;

private:
    Section_Settings() = default;
    ~Section_Settings() = default;
    Section_Settings(const Section_Settings&) = delete;
    Section_Settings& operator=(const Section_Settings&) = delete;

    std::map<std::string, std::vector<int>> sectionLengthRules;
    std::map<std::string, std::string> settingsMap;

    static const std::string DEFAULT_PATH;

    // small helpers
    static std::string trim(const std::string& s);
    static bool parseIntSafe(const std::string& s, int &out);
};

#endif // SECTION_SETTINGS_H
