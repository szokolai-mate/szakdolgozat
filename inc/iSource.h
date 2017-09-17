#pragma once

#include <vector>

namespace DataFlow {
    template <typename T>
    class iSource{
        public:
        virtual std::vector<T> get(const unsigned int & amount) = 0;
    };
}