#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class FinalSummaryManager {
public:

    void clear() {
        windows.clear();
    }

    // Set input rates
    void setGlassRate(double rate);
    void setLaborRate(double rate);
    void setHardwareRate(double rate);
    void setAluminiumDiscount(double percentage);
    void setAluminiumTotal(double total);

    // Optional details
    void setProjectName(const std::string& name);
    void setLocation(const std::string& loc);
    void setPhoneNumber(const std::string& phone);
    void setGlassColor(const std::string& color);
    void setGageNumber(const std::string& gage);
    void setAluColor(const std::string& color);
    void setContractor(const std::string& constractor);
    void setContect(const std::string& contect);

    std::string getGageNumber() const;

    // Window summary functions
    void addWindow(const std::string& label, double areaSqFt);
    void print_qty_sq() const;
    void removeWindowSummary(const std::string& label, double areaSqFt);

    // Summary + PDF helpers
    void reset();
    
    // Changed from int ➜ void
    void displayFinalSummary() const;

    // New separated functions
    void generateInvoicePDF() const;
    // void generateMaterialPDF() const;
    // void generateCuttingPDF() const;

private:

    double glassRatePerSqFt = 0.0;
    double laborRatePerSqFt = 0.0;
    double hardwareRatePerWindow = 0.0;
    double aluminiumDiscountPercent = 0.0;
    double aluminiumTotalCost = 0.0;
    mutable double extra = 0.0;

    std::string projectName;
    std::string location;
    std::string phoneNumber;
    std::string gageNumber;
    std::string glassColor;
    std::string AluColor;
    std::string contractorName;
    std::string contractorphoneNumber;

    // These 3 can remain (safe)
    std::vector<std::pair<std::string, double>> windows;
    std::map<std::string, int> windowCounts;
    std::map<std::string, int> windowTypeCounts;

    // Main summary map used everywhere
    std::map<std::string, std::pair<int, double>> windowTypeSummary;
};
