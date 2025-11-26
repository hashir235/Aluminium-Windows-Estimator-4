#pragma once
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include "EstimateLengthManager.h"

//  Base class for ALL Frame components (S-series, F-series, etc.)
class FrameComponent {
protected:
    std::string label;
    double height = 0.0;
    double width = 0.0;
    int collarType = -1;   // Default -1 = not set
    int lockType   = -1;   // Added: For windows that require lock selection

    //  Consistent format: { SectionName -> vector of (length, label) }
    using SectionMap = std::unordered_map<std::string, std::vector<std::pair<double, std::string>>>;
    SectionMap sectionMap;

public:
    // 🔹 Input Data (each derived class will handle its own logic)
    virtual bool inputWindowData(bool isEditing = false) = 0;

    // 🔹 Perform all calculations for sections
    virtual void calculateSections() = 0;

    // 🔹 Area (sqft) calculation (optional for some frames, pure virtual here)
    virtual double getAreaSqFt() const = 0;

    // 🔹 Access SectionMap
    virtual const SectionMap& getSectionMap() const { return sectionMap; }

    // 🔹 Estimator integration
    virtual void sendToEstimator(EstimateLengthManager& estimator) = 0;
    virtual void sendToEstimator() = 0;  // Optional overload (warn if not used)

    // 🔹 Generic getters
    virtual std::string getLabel() const { return label; }
    virtual double getHeight() const { return height; }
    virtual double getWidth()  const { return width; }
    virtual int getCollarType() const { return collarType; }
    virtual int getLockType()   const { return lockType; }  // ✅ New for lock-based windows

    // 🧹 Virtual destructor
    virtual ~FrameComponent() = default;
};
