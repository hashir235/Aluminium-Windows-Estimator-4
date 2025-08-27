#pragma once

#include "FrameComponent.h"
#include "CommonTypes.h"
#include "EstimateLengthManager.h"
#include <string>

class SGM_Win : public FrameComponent {
private:
    double height;
    double width;
    int collarType;
    bool inputInInches;
    std::string label;
    SectionMap sectionMap;
    int glassOption; // 1, 2, or 3
    char unitChoice;
    static int windowCount; // Static variable to count windows

    // Helper methods for input
    bool selectGlassOption();
    bool inputSizeAndCollar();

public:
    SGM_Win();
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
};
