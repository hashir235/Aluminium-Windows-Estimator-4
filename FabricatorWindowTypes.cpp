#include "FabricatorWindowTypes.h"

std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> FabricatorWindowTypes = {
    {1, {"Simple Window", [](){ return std::make_unique<S_Win_f>(); }}}
    // {2, {"Sliding Window", [](){ return std::make_unique<SM_Win_f>(); }}},
    // {3, {"Single Glass Window", [](){ return std::make_unique<SG_Win_f>(); }}},
    // {4, {"Single Glass + Mesh Window", [](){ return std::make_unique<SGM_Win_f>(); }}},
    // {5, {"Simple Casement Window", [](){ return std::make_unique<SC_Win_f>(); }}},
    // {6, {"Simple Casement + Mesh", [](){ return std::make_unique<SCM_Win_f>(); }}},
    // {7, {"Fixed Window", [](){ return std::make_unique<F_Win_f>(); }}},
    // {8, {"Fixed Casement Window", [](){ return std::make_unique<FC_Win_f>(); }}},
    // {9, {"Openable Window", [](){ return std::make_unique<O_Win_f>(); }}},
    // {10, {"Double Window", [](){ return std::make_unique<D_Win_f>(); }}},
    // {11, {"Arch Window", [](){ return std::make_unique<A_Win_f>(); }}}
};

