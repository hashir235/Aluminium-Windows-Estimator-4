#ifndef D_Win_f_H
#define D_Win_f_H

#include "CommonTypes.h"
#include "FrameComponent.h"  // Must inherit from this
#include <string>
#include <vector>
#include <map>

class EstimateLengthManager;

class D_Win_f : public FrameComponent { // <-- important inheritance
public:
    D_Win_f();
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
    int getCollarType() const;
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    // Send to estimator
    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator(); // Warning if no estimator

private:
    double height;
    double width;
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

#endif // D_Win_f_H
