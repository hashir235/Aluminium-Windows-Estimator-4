#pragma once
#include "FrameComponent.h"
#include "CommonTypes.h"
#include "EstimateLengthManager.h"
#include <string>
#include <map>
#include <vector>

class SM_Win_e : public FrameComponent {
public:
    SM_Win_e();
    static void resetWindowCount();
    bool inputWindowData(bool isEditing = false) override;
    void calculateSections() override;
    double getAreaSqFt() const override;
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator() override;

    char getSize() const; // Get size unit (inches/feet)
    double getHeight() const;
    double getWidth() const;
    int getCollarType() const;

private:
    double height;
    double width;
    int collarType;
    bool inputInInches;
    std::string label;
    SectionMap sectionMap;
    char unitChoice;
    static int windowCount; // Static variable to count windows
};
