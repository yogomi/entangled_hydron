// Copyright 2014 Makoto Yano

#include <cstdint>
#include <list>
#include <algorithm>

#include "include/hydron.h"

namespace hydron {

Hydron::Hydron()
  : id_(HydronId())
  , threshold_(0)
  , strength_(1)
  , step_(0)
  , refractory_period_(0) {}

Hydron::Hydron(const int32_t x, const int32_t y, const int32_t z)
  :id_(HydronId(x, y, z))
  , threshold_(0)
  , strength_(1)
  , step_(0)
  , refractory_period_(0) {}

void Hydron::ChangeId(const int32_t x, const int32_t y, const int32_t z) {
  id_ = HydronId(x, y, z);
}

void Hydron::ShowStatus() {
  printf("ID: (%d, %d, %d); ", id_.x(), id_.y(), id_.z());
  printf("threshold: %d; ", threshold_);
  printf("strength: %d; ", strength_);
  printf("step: %d; ", step_);
  printf("refractory period: %d; ", refractory_period_);
  printf("connectiong hydron count: %lu; ", connecting_hydron_.size());
  printf("\n");
}

}  // namespace hydron
