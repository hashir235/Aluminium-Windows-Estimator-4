#include "EstimatorWindowTypes.h"

std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> EstimatorWindowTypes = {
    {1, {"Sliding Window", [] { return std::make_unique<S_Win_e>(); }}},
    {2, {"Sliding Window (M section)", [] { return std::make_unique<SM_Win_e>(); }}},
    {3, {"3/4 Glass part Window", [] { return std::make_unique<SG_Win_e>(); }}},
    {4, {"3/4 Glass part Window (M section)", [] { return std::make_unique<SGM_Win_e>(); }}},
    {5, {"Cornner Slide Window", [] { return std::make_unique<SC_Win_e>(); }}},
    {6, {"Cornner Slide Window (M section)", [] { return std::make_unique<SCM_Win_e>(); }}},
    {7, {"Fix Window", [] { return std::make_unique<F_Win_e>(); }}},
    {8, {"Corrner Fix Window", [] { return std::make_unique<FC_Win_e>(); }}},
    {9, {"Openable Window", [] { return std::make_unique<O_Win_e>(); }}},
    {10, {"Door", [] { return std::make_unique<D_Win_e>(); }}},
    {11, {"Arch Window", [] { return std::make_unique<A_Win_e>(); }}}
};
