#pragma once

#include <math.h>
#include <vector>

#include <iSource.h>
#include <Utils.h>
//TODO: változtatható frekvencia

//TODO: NOTE: sample rate is fixed to 48000
#define SINE_TABLE_SIZE 2400

namespace Mixer{
    template <typename T>
    class SineGenerator: public DataFlow::iSource<T>{
        public:
            std::vector<T> get(const unsigned int & amount);

            SineGenerator(const float& frequency);

            constexpr static const int table_size = SINE_TABLE_SIZE;
            static const std::array<float,table_size> sinetable;

        private:
            float step;
            unsigned int position;

            static float functor(double x) { return (float)sin(((double)x / (double)SineGenerator::table_size) * M_PI * 2.0); }

    };
}