// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <cstdint>
#include <list>
#include <map>

#include "./vector.h"

namespace hydron {

typedef common3d::Vector HydronId;
typedef std::list<HydronId> HydronIdList;

struct HydronParameter {
  float temperature;
  float threshold;
  float strength;
  float radiation_ability;
  uint32_t refractory_period;
};

struct HydronConnection {
  HydronId id;
  float weight;
};

class Hydron {
 public:
  Hydron();
  Hydron(const float x, const float y, const float z);
  void RegisterToAllHydronMap();

  void Fire();
  void AddHeat(const float &heat);

  uint32_t ChangeId(const float x, const float y, const float z);
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

  void ConnectTo(const float x
                , const float y
                , const float z
                , const float weight = 1);
  void ConnectTo(const HydronId &id, const float weight = 1);
  void ConnectTo(const Hydron& h, const float weight = 1);

  std::list<struct HydronConnection> ConnectingHydrons() const;

  HydronId Id() const {
    return id_;
  }
  bool IsZeroHydron() const {
    return (id_.x() == id_.y() == id_.z() == 0);
  }

  void ExportStatus(FILE *file) const;
  void ShowStatus() const;

 private:
  HydronId id_;
  common3d::Vector head_direction_;
  struct HydronParameter parameter_;
  float temperature_buffer_;
  std::list<struct HydronConnection> connecting_hydrons_;
  static std::map<HydronId, Hydron *> all_hydron_map_;
};

}  // namespace hydron

#endif  // INCLUDE_HYDRON_H_
