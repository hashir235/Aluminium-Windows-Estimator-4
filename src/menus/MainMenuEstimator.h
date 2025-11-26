#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <limits>
#include <ctime>
#include <sstream>
#include <iomanip>

// Project-specific headers
#include "FrameComponent.h"
#include "FinalSummaryManager.h"
#include "FinalCostCalculator.h"
#include "EstimateLengthManager.h"
#include "Section_Settings.h"
#include "general_settings.h" 
#include "Margins_Settings.h" 
#include "EstimatorManager.h"  
#include "EstimatorWindowTypes.h"
#include "ManualRatesManager.h"
#include "AutoRatesManager.h"
#include "CuttingSize.h"

// Window type header files
#include "S_Win_e.h"
#include "SM_Win_e.h"
#include "SG_Win_e.h"
#include "SGM_Win_e.h"
#include "SC_Win_e.h"
#include "SCM_Win_e.h"
#include "F_Win_e.h"
#include "FC_Win_e.h"
#include "O_Win_e.h"
#include "D_Win_e.h"
#include "A_Win_e.h"

using namespace std;


// Case 1: Add Window for Estimation
void addOrEditWindowsLoop(
    vector<unique_ptr<FrameComponent>>& windows,
    map<int, pair<string, function<unique_ptr<FrameComponent>()>>>& EstimatorWindowTypes,
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

void getCuttingSize(int);  // choice 5

// Case 3: Settings
void settingsMenu();
