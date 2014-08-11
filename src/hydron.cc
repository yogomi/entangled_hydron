// Copyright 2014 Makoto Yano

#include <cstdint>
#include <list>
#include <algorithm>

#include "./vector.h"
#include "x64_x32/format_type.h"
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

Hydron::Hydron(const float x, const float y, const float z)
  :id_(HydronId(x, y, z)) {
  SetParameter(0.0f, 0.0f, 1.0f, 1.0f, 0);
  SetHeadDirection(Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f));
}

Hydron::Hydron(const HydronId &id)
  : id_(id) {
  SetParameter(0.0f, 0.0f, 1.0f, 1.0f, 0);
  SetHeadDirection(Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f)
                , Random<float>(-1.0f, 1.0f));
}

Hydron::~Hydron() {
  if (all_hydron_map_[id_] == this) {
    all_hydron_map_.erase(id_);
    neighbor_hydron_searcher_.RemoveVector(id_);
  }
}

void Hydron::RegisterToAllHydronMap() {
  all_hydron_map_[id_] = this;
  neighbor_hydron_searcher_.AddVector(id_);
}

void Hydron::Fire() {
  if (parameter_.temperature > parameter_.threshold) {
    auto FireNextHydron =
      [](const struct HydronConnection &connection) -> void {
      if (all_hydron_map_.find(connection.id) != all_hydron_map_.end()) {
        all_hydron_map_[connection.id]->AddHeat(connection.weight);
      }
    };
    for_each(connecting_hydrons_.begin()
          , connecting_hydrons_.end()
          , FireNextHydron);

    parameter_.temperature = 0.0f;
    refractory_period_ = parameter_.refractory_span;
  }
}

void Hydron::AddHeat(const float &heat) {
  temperature_buffer_ += heat;
}

void Hydron::AdaptHeatEffect() {
  if (refractory_period_ == 0) {
    parameter_.temperature =
      parameter_.temperature / parameter_.radiation_ability;
    parameter_.temperature += temperature_buffer_;
    temperature_buffer_ = 0.0f;
  } else {
    --refractory_period_;
  }
}

uint32_t Hydron::ChangeId(const float x, const float y, const float z) {
  HydronId new_id = HydronId(x, y, z);
  bool hydron_in_all_hydron_map =
    all_hydron_map_.find(id_) != all_hydron_map_.end();
  if (hydron_in_all_hydron_map) {
    if (all_hydron_map_.find(new_id) != all_hydron_map_.end()) {
      return -1;
    }
    all_hydron_map_.erase(id_);
  }
  id_ = new_id;
  if (hydron_in_all_hydron_map) {
    RegisterToAllHydronMap();
  }
  return 0;
}

void Hydron::SetHeadDirection(const float x, const float y, const float z) {
  head_direction_ = common3d::Normalize(common3d::Vector(x, y, z));
}


void Hydron::SetParameter(const float temperature
                      , const float threshold
                      , const float strength
                      , const float radiation_ability
                      , const uint32_t refractory_span) {
  parameter_.temperature = temperature;
  parameter_.threshold = threshold;
  parameter_.strength = strength;
  parameter_.radiation_ability = radiation_ability;
  parameter_.refractory_span = refractory_span;
  temperature_buffer_ = 0;
  refractory_period_ = 0;
}

void Hydron::SetParameter(const struct HydronParameter &parameter) {
  parameter_ = parameter;
  temperature_buffer_ = 0;
  refractory_period_ = 0;
}

void Hydron::ConnectTo(const float x
                      , const float y
                      , const float z
                      , const float weight) {
  ConnectTo(HydronId(x, y, z), weight);
}

void Hydron::ConnectTo(const HydronId &id, const float weight) {
  struct HydronConnection next_hydron;
  next_hydron.id = id;
  next_hydron.weight = weight;
  connecting_hydrons_.push_back(next_hydron);
}

void Hydron::ConnectTo(const Hydron &h, const float weight) {
  ConnectTo(h.Id(), weight);
}

std::list<struct HydronConnection> Hydron::ConnectingHydrons() const {
  return connecting_hydrons_;
}

void Hydron::ExportStatus(FILE *file) const {
  auto ExportFloat = [&file](const float value) -> void {
    fwrite(&value, sizeof(value), 1, file);
  };
  auto ExportVector = [&file, &ExportFloat](const common3d::Vector &v) -> void {
    ExportFloat(v.x());
    ExportFloat(v.y());
    ExportFloat(v.z());
  };
  ExportVector(id_);
  ExportVector(head_direction_);
  fwrite(&parameter_, sizeof(parameter_), 1, file);
  uint64_t connecting_hydrons_count = connecting_hydrons_.size();
  fwrite(&connecting_hydrons_count, sizeof(connecting_hydrons_count), 1, file);
  std::for_each(connecting_hydrons_.begin()
              , connecting_hydrons_.end()
              , [&ExportVector, &ExportFloat](
                const struct HydronConnection &hydron_connect) {
    ExportVector(hydron_connect.id);
    ExportFloat(hydron_connect.weight);
  });
}

void Hydron::ShowStatus() const {
  printf("ID: (%f, %f, %f); ", id_.x(), id_.y(), id_.z());
  printf("Head Direction: (%f, %f, %f)"
        , head_direction_.x()
        , head_direction_.y()
        , head_direction_.z());
  printf("temperatur: %f; ", parameter_.temperature);
  printf("temperatur_buffer: %f; ", temperature_buffer_);
  printf("threshold: %f; ", parameter_.threshold);
  printf("strength: %f; ", parameter_.strength);
  printf("radiation ability: %f; ", parameter_.radiation_ability);
  printf("refractory span: %d; ", parameter_.refractory_span);
  printf("refractory period: %d; ", refractory_period_);
  printf("connectiong hydron count: %" PRIuS "; ", connecting_hydrons_.size());
  printf("connecting hydron ids: ");
  std::for_each(connecting_hydrons_.begin()
              , connecting_hydrons_.end()
              , [](const struct HydronConnection &hydron_connect) {
    HydronId id = hydron_connect.id;
    printf("(%f, %f, %f) ", id.x(), id.y(), id.z());
    printf("weight: %f; ", hydron_connect.weight);
  });
  printf("\n");
}

std::map<HydronId, Hydron *> Hydron::all_hydron_map_;
common3d::BlockGrid Hydron::neighbor_hydron_searcher_;

}  // namespace hydron
