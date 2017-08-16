#include <iBuffer.h>

namespace DataFlow {
    template <typename T>
    class iSource{
        public:
        virtual bool get(const unsigned int & amount, iBuffer<T>& buffer) = 0;
    }
}