#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "FrameComponent.h"

// Fabricator ke liye windows management
void printFabricatorWindowsSummary(const std::vector<std::unique_ptr<FrameComponent>>& windows);
void addWindowForFabrication(
    std::vector<std::unique_ptr<FrameComponent>>& windows,
    std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>>& windowTypes
);
void editWindowFabricator(std::vector<std::unique_ptr<FrameComponent>>& windows);
void deleteWindowFabricator(std::vector<std::unique_ptr<FrameComponent>>& windows);

// Extra features
void getMaterialNeeded();
void getCuttingSize();
void settingsFabricator();
