#pragma once

#include <cassert>

namespace Ignosi::Modules
{
    template <typename D_T, typename B_T>
    D_T DebugCast(B_T pBase)
    {
#ifdef DEBUG
        D_T pDerived = dynamic_cast<D_T>(pBase);
        assert(pDerived);
        return pDerived;
#else
        return reinterpret_cast<D_T>(pBase);
#endif
    }
} // namespace Ignosi::Modules