// Copyright 2014 Makoto Yano

#include <cstdint>
#include <list>
#include <algorithm>

#include "./vector.h"
#include "./random_number_generator.h"
#include "./hydron.h"

namespace hydron {

Hydron::Hydron()
  : id_(HydronId()) {
  SetParameter(0.0f, 0.0f, 1.0f, 1.0f, 0);
  SetHeadDirection(Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f));
}

Hydron::Hydron(const int32_t x, const int32_t y, const int32_t z)
  :id_(HydronId(x, y, z)) {
  SetParameter(0.0f, 0.0f, 1.0f, 1.0f, 0);
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


void Hydron::SetParameter(const float temperature
                      , const float threshold
                      , const float strength
                      , const float radiation_ability
                      , const uint32_t refractory_period) {
  parameter_.temperature = temperature;
  parameter_.threshold = threshold;
  parameter_.strength = strength;
  parameter_.radiation_ability = radiation_ability;
  parameter_.refractory_period = refractory_period;
}

void Hydron::SetParameter(const struct HydronParameter &parameter) {
  parameter_ = parameter;
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

void Hydron::ExportStatus(FILE *file) {
  auto ExportVector = [&](const common3d::Vector &v) -> void {
    auto ExportFloat = [&](const float value) -> void {
      fwrite(&value, sizeof(value), 1, file);
    };
    ExportFloat(v.x());
    ExportFloat(v.y());
    ExportFloat(v.z());
  };
  ExportVector(id_);
  ExportVector(head_direction_);
  fwrite(&parameter_, sizeof(parameter_), 1, file);
  uint64_t connecting_hydron_count = connecting_hydron_.size();
  fwrite(&connecting_hydron_count, sizeof(connecting_hydron_count), 1, file);
  std::for_each(connecting_hydron_.begin()
              , connecting_hydron_.end()
              , [&](const HydronId &id) {
    ExportVector(id);
  });
}

void Hydron::ShowStatus() {
  printf("ID: (%f, %f, %f); ", id_.x(), id_.y(), id_.z());
  printf("Head Direction: (%f, %f, %f)"
        , head_direction_.x()
        , head_direction_.y()
        , head_direction_.z());
  printf("temperatur: %f; ", parameter_.temperature);
  printf("threshold: %f; ", parameter_.threshold);
  printf("strength: %f; ", parameter_.strength);
  printf("radiation ability: %f; ", parameter_.radiation_ability);
  printf("refractory period: %d; ", parameter_.refractory_period);
  printf("connectiong hydron count: %lu; ", connecting_hydron_.size());
  printf("connecting hydron ids: ");
  std::for_each(connecting_hydron_.begin()
              , connecting_hydron_.end()
              , [](const HydronId &id) {
    printf("(%f, %f, %f) ", id.x(), id.y(), id.z());
  });
  printf("\n");
}

}  // namespace hydron
