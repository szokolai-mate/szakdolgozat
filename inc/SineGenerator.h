#include <iSource.h>

namespace Mixer{
    template <typename T>
    class SineGenerator: iSource<T>{
        public:
            bool get(const unsigned int & amount, iBuffer<T>& buffer);

            SineGenerator(const float& frequency);

        private:
            unsigned short position, step;

    }
}