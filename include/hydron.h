// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <cstdint>
#include <list>

#include "include/vector.h"

namespace hydron {

typedef common3d::Vector HydronId;

struct HydronParameter {
  float temperature;
  float threshold;
  float strength;
  float radiation_ability;
  uint32_t refractory_period;
};

class Hydron {
 public:
  Hydron();
  Hydron(const int32_t x, const int32_t y, const int32_t z);

  void ChangeId(const int32_t x, const int32_t y, const int32_t z);
  void SetHeadDirection(const float x, const float y, const float z);

  void SetParameter(const float temperature
                , const float threshold
                , const float strength
                , const float radiation_ability
                , const uint32_t refractory_period);
  void SetParameter(const struct HydronParameter &parameter);
  struct HydronParameter Parameter() const {
    return parameter_;
  }

  ~Hydron() {}

  void ConnectTo(const int32_t x, const int32_t y, const int32_t z);
  void ConnectTo(const HydronId &id);
  void ConnectTo(const Hydron& h);

  HydronId Id() const {
    return id_;
  }
  void ShowStatus();

 private:
  HydronId id_;
  common3d::Vector head_direction_;
  struct HydronParameter parameter_;
  std::list<HydronId> connecting_hydron_;
};

}  // namespace hydron

#endif  // INCLUDE_HYDRON_H_
