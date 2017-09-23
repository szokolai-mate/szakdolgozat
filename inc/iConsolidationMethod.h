#pragma once

#include <vector>

namespace DataFlow{
    template <typename T>
    class iConsolidationMethod {
    public:
        virtual bool init(const std::vector<T> & initial) = 0;
        virtual bool add(const std::vector<T> & vector) = 0;
        virtual std::vector<T> get() = 0;
        virtual ~iConsolidationMethod(){};
    };
};