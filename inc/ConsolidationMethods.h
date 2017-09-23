#pragma once

#include <iConsolidationMethod.h>

namespace Consolidation{
    template <typename T>
    class Accumulation : public DataFlow::iConsolidationMethod<T>{
    private:
        std::vector<T> workspace;
    public:
        bool init(const std::vector<T> & initial);
        bool add(const std::vector<T> & vector);
        std::vector<T> get();
    };
};