#pragma once
#include <map>
#include <string>
#include <vector>
#include "EstimateLengthManager.h"

class FrameComponent {
protected:
    std::string label;
    double height = 0;
    double width = 0;
    int collarType = 0;
    std::map<std::string, std::vector<double>> sectionMap;

public:
    // 🔹 Core interface methods
    virtual bool inputWindowData(bool isEditing = false) = 0;
    virtual void calculateSections() = 0;
    virtual double getAreaSqFt() const = 0;
    virtual const std::unordered_map<std::string, std::vector<std::pair<double, std::string>>>& getSectionMap() const = 0;

    // 🔸 Dual Estimator Interface (Overloaded)
    virtual void sendToEstimator(EstimateLengthManager& estimator) = 0;
    virtual void sendToEstimator() = 0;
 
    virtual std::string getLabel() const = 0;
    virtual double getHeight() const = 0;
    virtual double getWidth() const = 0;
    virtual int getCollarType() const = 0;  // or -1 if not applicable

    // 🧹 Ensure proper cleanup in derived classes
    virtual ~FrameComponent() = default;
};
