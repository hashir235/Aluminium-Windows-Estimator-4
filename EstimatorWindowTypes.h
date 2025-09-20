#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "FrameComponent.h"
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

extern std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> EstimatorWindowTypes;
