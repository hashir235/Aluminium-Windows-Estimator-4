#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <limits>

// Project-specific headers
#include "FrameComponent.h"
#include "FinalSummaryManager.h"
#include "FinalCostCalculator.h"
#include "EstimateLengthManager.h"
#include "SettingsManagerA.h"
#include "SettingsManagerB.h" 
#include "SettingsManagerC.h" 
#include "WindowManager.h"  
#include "WindowTypes.h"
#include "ManualRatesManager.h"
#include "AutoRatesManager.h"

// Window type header files
#include "S_Win.h"
#include "SM_Win.h"
#include "SG_Win.h"
#include "SGM_Win.h"
#include "SC_Win.h"
#include "SCM_Win.h"
#include "F_Win.h"
#include "FC_Win.h"
#include "O_Win.h"
#include "D_Win.h"
#include "A_Win.h"

using namespace std;


// Case 1: Add Window for Estimation
void addOrEditWindowsLoop(
    vector<unique_ptr<FrameComponent>>& windows,
    map<int, pair<string, function<unique_ptr<FrameComponent>()>>>& windowTypes,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,
    FinalSummaryManager& summaryManager,
    FinalCostCalculator& finalEstimator,
    AutoRatesManager& autoRatesManager
);


// Case 2: Show Final Summary
void showFinalSummary(
    vector<unique_ptr<FrameComponent>>& windows,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,
    FinalSummaryManager& summaryManager,
    FinalCostCalculator& finalEstimator,
    AutoRatesManager& autoRatesManager
);



// Case 3: Add Window for Fabrication
void addWindowForFabrication();

// Case 4: Get Cutting Size
void getCuttingSize();

// Case 5: Settings
void settingsMenu();
