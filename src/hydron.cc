// Copyright 2014 Makoto Yano

#include <cstdint>

#include "include/hydron.h"

namespace hydron {

Hydron::Hydron()
  :id_(HydronId()), step_(0), threshold_(0), strength_(1) {}

Hydron::Hydron(const int32_t x, const int32_t y, const int32_t z)
  :id_(HydronId(x, y, z)), step_(0), threshold_(0), strength_(1) {}

void Hydron::ChangeId(const int32_t x, const int32_t y, const int32_t z) {
  id_ = HydronId(x, y, z);
}

}  // namespace hydron
