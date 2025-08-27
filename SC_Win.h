#ifndef SC_WIN_H
#define SC_WIN_H

#include "CommonTypes.h"
#include "FrameComponent.h"  // Must inherit from this
#include <string>
#include <vector>
#include <map>

class EstimateLengthManager;

class SC_Win : public FrameComponent { // <-- important inheritance
public:
    SC_Win();

    // User input
    bool inputWindowData(bool isEditing = false) override;

    // Calculations
    void calculateSections() override;

    // Getters
    char getSize() const ; // Get size unit (inches/feet)
    double getAreaSqFt() const override;
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

#endif // SC_WIN_H
