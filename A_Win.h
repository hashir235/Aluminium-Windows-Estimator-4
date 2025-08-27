#ifndef A_Win_H
#define A_Win_H

#include "CommonTypes.h"
#include "FrameComponent.h"  // Must inherit from this
#include <string>
#include <vector>
#include <map>

class EstimateLengthManager;

class A_Win : public FrameComponent { // <-- important inheritance
public:
    A_Win();
    static void resetWindowCount();
    // User input
    bool inputWindowData(bool isEditing = false) override;

    // Calculations
    void calculateSections() override;

    // Getters
    char getSize() const; // Get size unit (inches/feet)
    double getAreaSqFt() const override;
    double getHeight() const;
    double getWidth() const;
    double getArch() const; 
    int getCollarType() const;
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    // Send to estimator
    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator(); // Warning if no estimator

private:
    double height;
    double width;
    double arch; 
    int collarType;
    int winType;
    bool inputInInches;
    std::string label;
    std::string bottom;
    std::string Tee;
    SectionMap sectionMap;
    char unitChoice;
    static int windowCount; // Static variable to count windows
    // New private helper functions for input flow
    bool selectWindowType();
    bool inputDetails();
};

#endif // A_Win_H
