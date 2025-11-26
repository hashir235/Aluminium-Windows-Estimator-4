#pragma once

#include "FrameComponent.h"
#include "CommonTypes.h"
#include "EstimateLengthManager.h"

class S_Win_f : public FrameComponent {
private:
    std::string label;
    SectionMap sectionMap;
    static int windowCount;
    int lockType;   // ✔ lock type
public:
    S_Win_f();
    static void resetWindowCount();
    bool inputWindowData(bool isEditing = false) override;
    void calculateSections() override;

    //double getAreaSqFt() const override;  // TODO: abhi implement karna hoga
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator() override;

    double getAreaSqFt() const override;  // signature only

    // ✔ Generic getters
    double getHeight() const;
    double getWidth() const;
    int getCollarType() const;
    int getLockType() const ;
};
