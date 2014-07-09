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

void Hydron::ConnectTo(const int32_t x, const int32_t y, const int32_t z) {
  connecting_hydron_.push_back(HydronId(x, y, z));
}

void Hydron::ConnectTo(const HydronId &id) {
  connecting_hydron_.push_back(id);
}

void Hydron::ConnectTo(const Hydron &h) {
  connecting_hydron_.push_back(h.Id());
}

void Hydron::ShowStatus() {
  printf("ID: (%d, %d, %d); ", id_.x(), id_.y(), id_.z());
  printf("threshold: %d; ", threshold_);
  printf("strength: %d; ", strength_);
  printf("step: %d; ", step_);
  printf("refractory period: %d; ", refractory_period_);
  printf("connectiong hydron count: %u; ", connecting_hydron_.size());
  printf("connecting hydron ids: ");
  std::for_each(connecting_hydron_.begin()
              , connecting_hydron_.end()
              , [](HydronId &id) {
    printf("(%d, %d, %d) ", id.x(), id.y(), id.z());
  });
  printf("\n");
}

}  // namespace hydron
