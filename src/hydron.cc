// Copyright 2014 Makoto Yano

#include <cstdint>
#include <list>
#include <algorithm>

#include "include/vector.h"
#include "include/random_number_generator.h"
#include "include/hydron.h"

namespace hydron {

Hydron::Hydron()
  : id_(HydronId())
  , temperature_(0.0f)
  , threshold_(0.0f)
  , strength_(1.0f)
  , radiation_ability_(1.0f)
  , refractory_period_(0) {
  SetHeadDirection(Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f));
}

Hydron::Hydron(const int32_t x, const int32_t y, const int32_t z)
  :id_(HydronId(x, y, z))
  , temperature_(0.0f)
  , threshold_(0.0f)
  , strength_(1.0f)
  , radiation_ability_(1.0f)
  , refractory_period_(0) {
  SetHeadDirection(Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f));
}

void Hydron::ChangeId(const int32_t x, const int32_t y, const int32_t z) {
  id_ = HydronId(x, y, z);
}

void Hydron::SetHeadDirection(const float x, const float y, const float z) {
  head_direction_ = common3d::Normalize(common3d::Vector(x, y, z));
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
  printf("ID: (%f, %f, %f); ", id_.x(), id_.y(), id_.z());
  printf("Head Direction: (%f, %f, %f)"
        , head_direction_.x()
        , head_direction_.y()
        , head_direction_.z());
  printf("temperatur: %f; ", temperature_);
  printf("threshold: %f; ", threshold_);
  printf("strength: %f; ", strength_);
  printf("radiation ability: %f; ", radiation_ability_);
  printf("refractory period: %d; ", refractory_period_);
  printf("connectiong hydron count: %lu; ", connecting_hydron_.size());
  printf("connecting hydron ids: ");
  std::for_each(connecting_hydron_.begin()
              , connecting_hydron_.end()
              , [](HydronId &id) {
    printf("(%f, %f, %f) ", id.x(), id.y(), id.z());
  });
  printf("\n");
}

}  // namespace hydron
