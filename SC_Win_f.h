#ifndef SC_Win_f_H
#define SC_Win_f_H

#include "CommonTypes.h"
#include "FrameComponent.h"
#include <string>
#include <vector>
#include <map>

class EstimateLengthManager;

class SC_Win_f : public FrameComponent {
public:
    SC_Win_f();

    // --- User Input & Configuration ---
    bool inputWindowData(bool isEditing = false) override;

    // --- Calculations ---
    void calculateSections() override;

    // --- Getters ---
                      // size unit (inches/feet)
    double getAreaSqFt() const override;
    double getHeight() const;
  
    double getRightWidth() const;
    double getLeftWidth() const;
    int getCollarType() const;
    int getWindowType() const;               // ✅ new getter for Right/Left Fix type
    std::string getLabel() const override;
    const SectionMap& getSectionMap() const override;

    // --- Estimator Integration ---
    void sendToEstimator(EstimateLengthManager& estimator) override;
    void sendToEstimator(); // Warning if no estimator
    static void resetWindowCount();

private:
    // --- Window dimensions ---
    double height;
    double width;
    double rightWidth;
    double leftWidth;

    // --- Window configuration ---
    int collarType;      // 1 = normal, etc.
    int winType;         // ✅ 0 = Left Fix, 1 = Right Fix (we’ll define in cpp)
    bool inputInInches;
    std::string label;
    char unitChoice;
    SectionMap sectionMap;
     int lockChoice = 0;

    static int windowCount;

    // --- Helper Input Functions ---
    bool selectWindowType();     // ✅ lets user choose Left/Right Fix
    bool selectLockType();       // handles lock type selection
    bool inputDetails();         // handles measurements after type selection

    // --- Optional Future Helpers ---
    void showTypeSummary() const;  // ✅ optional for debugging or menu display
};

#endif // SC_Win_f_H
