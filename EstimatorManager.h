#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <vector>
#include <memory>

#include "FrameComponent.h"        // Base class for all windows
#include "EstimateLengthManager.h" // Estimation logic
#include "FinalCostCalculator.h"   // Cost calculation
#include "FinalSummaryManager.h"   // Summary handling

// EstimatorManager ka kaam hai: Add, Edit, Delete windows handle karna

// Add a new window
void addWindow(std::vector<std::unique_ptr<FrameComponent>>& windows,
               EstimateLengthManager& estimator,
               FinalSummaryManager& summaryManager);

// Edit an existing window
void editWindow(std::vector<std::unique_ptr<FrameComponent>>& windows,
                EstimateLengthManager& estimator,
                FinalSummaryManager& summaryManager);

// Delete a window
void deleteWindow(std::vector<std::unique_ptr<FrameComponent>>& windows,
                  EstimateLengthManager& estimator,
                  FinalSummaryManager& summaryManager);

#endif // WINDOW_MANAGER_H

