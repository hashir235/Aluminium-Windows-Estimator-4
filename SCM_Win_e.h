#ifndef SCM_Win_e_H
#define SCM_Win_e_H

#include "CommonTypes.h"
#include "FrameComponent.h"  // Must inherit from this
#include <string>
#include <vector>
#include <map>

class EstimateLengthManager;

class SCM_Win_e : public FrameComponent { // <-- important inheritance
public:
    SCM_Win_e();

    // User input
    bool inputWindowData(bool isEditing = false) override;

    // Calculations
    void calculateSections() override;

    // Getters
    char getSize() const ; // Get size unit (inches/feet)
    double getAreaSqFt() const;
    double getHeight() const;
    double getWidth() const;
    double getRightWidth() const;
    double getLeftWidth() const;
    int getCollarType() const;
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    // Send to estimator
    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator(); // Warning if no estimator
    static void resetWindowCount();

private:
    double height;
    double width;
    double rightWidth;
    double leftWidth;
    int collarType;
    int winType;
    bool inputInInches;
    std::string label;
    SectionMap sectionMap;
    char unitChoice;
    static int windowCount;

    // New private helper functions for input flow
    bool selectWindowType();
    bool inputDetails();
};

#endif // SCM_Win_e_H
