#pragma once

#include "FrameComponent.h"
#include "CommonTypes.h"         // for SectionMap
#include "EstimateLengthManager.h"  // forward declaration OR include if needed

class F_Win_f : public FrameComponent {
private:
    std::string label;
    SectionMap sectionMap;
    bool inputInInches;
    char unitChoice;
    static int windowCount;

public:
    F_Win_f();
    static void resetWindowCount();
    bool inputWindowData(bool isEditing = false) override;
    void calculateSections() override;

    //double getAreaSqFt() const override;
    std::string getLabel() const override;

    //const std::unordered_map<std::string, std::vector<std::pair<double, std::string>>>& getSectionMap() const override;
    const SectionMap& getSectionMap() const override;

    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator() override;

    std::string getSectionName(const std::string& sectionKey) const;

    char getSize() const;
    double getHeight() const;
    double getWidth() const;
    int getCollarType() const;
    double getAreaSqFt() const;

};
