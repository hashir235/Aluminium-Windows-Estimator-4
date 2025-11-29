#pragma once
#include <map>
#include <string>

class FinalCostCalculator {

private:
    std::string projectName;
    std::string location;
    std::string phoneNumber;
    std::string glassColor;
    std::string gageNumber;
    std::string AluColor;
    std::string contractorName;
    std::string contractorphoneNumber;

public:
    FinalCostCalculator();

    
    void inputRates();
    void displayFinalSummary();

    void setProjectName(const std::string& name);
    void setLocation(const std::string& loc);
    void setPhoneNumber(const std::string& phone);
    void setGlassColor(const std::string& color);
    void setGageNumber(const std::string& gage);
    void setAluColor(const std::string& color);
    void setContractor(const std::string& constractor);
    void setContect(const std::string& contect);
    
    std::string getProjectName() const;
    std::string getLocation() const;
    std::string getPhoneNumber() const;

    // ✅ Add this line below:
    //void addWindowSummary(int count, double areaSqFt, const std::map<std::string, double>& sectionSummary);
};

