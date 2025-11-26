#pragma once
#include <vector>
#include <memory>
#include <limits>
#include <functional>
#include "FrameComponent.h"
#include "ManualRatesManager.h"
#include "AutoRatesManager.h"
#include "EstimateLengthManager.h" 
#include "FabricatorWindowTypes.h"
#include "CuttingSize.h"


// Fabricator ke menu ke functions
void addWindowForFabrication(
    std::vector<std::unique_ptr<FrameComponent>>& windows,
    std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>>& FabricatorWindowTypes
);

void getMaterialNeeded(
    std::vector<std::unique_ptr<FrameComponent>>& windows,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,
    AutoRatesManager& autoRatesManager
);

void getCuttingSize(int);

void settingsFabricator();   

// 🔹 New functions for editing and deleting
void editWindowFabricator(std::vector<std::unique_ptr<FrameComponent>>& windows);
void deleteWindowFabricator(std::vector<std::unique_ptr<FrameComponent>>& windows);
