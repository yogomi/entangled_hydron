// Copyright 2014 Makoto Yano

#include <random>

#include "./random_number_generator.h"

template <typename TYPE>
TYPE Random(TYPE min, TYPE max) {
  std::random_device seed;
  std::minstd_rand random(seed());
  std::uniform_int_distribution<TYPE> generator(min, max);
  return generator(random);
}

template <>
uint64_t Random<uint64_t>(uint64_t min, uint64_t max) {
  std::random_device seed;
  std::minstd_rand random(seed());
  std::uniform_int_distribution<uint64_t> generator(min, max);
  return generator(random);
}

template <>
float Random<float>(float min, float max) {
  std::random_device seed;
  std::minstd_rand random(seed());
  std::uniform_real_distribution<float> generator(min, max);
  return generator(random);
}
