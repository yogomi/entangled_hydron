// Copyright 2014 Makoto Yano

#ifndef INCLUDE_RANDOM_NUMBER_GENERATOR_H_
#define INCLUDE_RANDOM_NUMBER_GENERATOR_H_

#include <random>

template <typename TYPE>
TYPE Random(TYPE min, TYPE max) {
  std::random_device seed;
  std::minstd_rand random(seed());
  std::uniform_real_distribution<TYPE> generator(min, max);
  return generator(random);
}

#endif  // INCLUDE_RANDOM_NUMBER_GENERATOR_H_
