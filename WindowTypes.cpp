#include "WindowTypes.h"

std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>> windowTypes = {
    {1, {"Sliding Window", [] { return std::make_unique<S_Win>(); }}},
    {2, {"Sliding Window (M section)", [] { return std::make_unique<SM_Win>(); }}},
    {3, {"3/4 Glass part Window", [] { return std::make_unique<SG_Win>(); }}},
    {4, {"3/4 Glass part Window (M section)", [] { return std::make_unique<SGM_Win>(); }}},
    {5, {"Cornner Slide Window", [] { return std::make_unique<SC_Win>(); }}},
    {6, {"Cornner Slide Window (M section)", [] { return std::make_unique<SCM_Win>(); }}},
    {7, {"Fix Window", [] { return std::make_unique<F_Win>(); }}},
    {8, {"Corrner Fix Window", [] { return std::make_unique<FC_Win>(); }}},
    {9, {"Openable Window", [] { return std::make_unique<O_Win>(); }}},
    {10, {"Door", [] { return std::make_unique<D_Win>(); }}},
    {11, {"Arch Window", [] { return std::make_unique<A_Win>(); }}}
};
