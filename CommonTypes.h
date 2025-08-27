// CommonTypes.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

using SectionMap = std::unordered_map<std::string, std::vector<std::pair<double, std::string>>>;

// If you want unordered version in future, just add this:
// using UnorderedSectionMap = std::unordered_map<std::string, std::vector<std::pair<double, std::string>>>;
