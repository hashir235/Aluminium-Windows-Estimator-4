#include "FabricatorWindowTypes.h"

std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> FabricatorWindowTypes = {
    {1, {"Sliding Window", [](){ return std::make_unique<S_Win_f>(); }}},
    {2, {"Sliding Window + (M section)", [](){ return std::make_unique<SM_Win_f>(); }}},
    {3, {"Glass part Window", [](){ return std::make_unique<SG_Win_f>(); }}},
    {4, {"Glass part + (M section)", [](){ return std::make_unique<SGM_Win_f>(); }}},
    {5, {"Slide Corner Window", [](){ return std::make_unique<SC_Win_f>(); }}},
    {6, {"Slide Corner Window + (M section)", [](){ return std::make_unique<SCM_Win_f>(); }}},
    {7, {"Fixed Window", [](){ return std::make_unique<F_Win_f>(); }}},
    {8, {"Fixed Corner Window", [](){ return std::make_unique<FC_Win_f>(); }}},
    {9, {"Openable Window", [](){ return std::make_unique<O_Win_f>(); }}},
    {10,{"Single/Double Door", [](){ return std::make_unique<D_Win_f>(); }}},
};

