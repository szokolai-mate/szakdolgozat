#pragma once

#include <math.h>
#include <vector>

#include <iSource.h>
#include <AudioSource.h>
#include <Utils.h>
//TODO: változtatható frekvencia

//TODO: NOTE: sample rate is fixed to 48000
#define SINE_TABLE_SIZE 4800000

namespace Mixer
{
template <typename T>
class SineGenerator : public DataFlow::iSource<T>, public Mixer::AudioSource
{
private:
  float step;
  unsigned int position;
  static bool sinetableUninitialized;

  static constexpr float filler(float x) { return (float)sin(((double)x / (double)SINE_TABLE_SIZE) * M_PI * 2.0); }
  static std::array<float, SINE_TABLE_SIZE> sinetable;

public:
  std::vector<T> get(const unsigned int &amount);

  SineGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate);
};
};