#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "FrameComponent.h"

// ⚡ Fabricator windows classes include
#include "S_Win_f.h"
// #include "SM_Win_f.h"
// #include "SG_Win_f.h"
// #include "SGM_Win_f.h"
// #include "SC_Win_f.h"
// #include "SCM_Win_f.h"
// #include "F_Win_f.h"
// #include "FC_Win_f.h"
// #include "O_Win_f.h"
// #include "D_Win_f.h"
// #include "A_Win_f.h"

// ⚡ Fabricator ke liye alag map
extern std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> FabricatorWindowTypes;
