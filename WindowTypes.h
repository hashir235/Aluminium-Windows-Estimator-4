#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "FrameComponent.h"
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

extern std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> windowTypes;
