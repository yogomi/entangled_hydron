// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <cstdint>
#include <list>
#include <map>

#include "./vector.h"
#include "./neighborhood_map.h"

namespace hydron {

typedef common3d::Vector HydronId;
typedef std::list<HydronId> HydronIdList;

struct HydronParameter {
  uint8_t flag;
  float temperature;
  float threshold;
  float strength;
  float radiation_ability;
  uint32_t refractory_span;
  uint32_t refractory_period;

  void SetNotValid() {
    flag = flag | 0x01;
  }
  bool IsValid() {
    uint8_t tmp = flag ^ 0xFE;
    tmp = ~tmp;
    return tmp > 0;
  }
};

struct HydronConnection {
  HydronId id;
  float weight;
};

class Hydron {
 public:
  Hydron();
  Hydron(const float x, const float y, const float z);
  explicit Hydron(const HydronId &id);
  void RegisterToAllHydronMap();

  void Fire();
  void AddHeat(const float &heat);
  void AdaptHeatEffect();

  uint32_t ChangeId(const float x, const float y, const float z);

  void SetParameter(const uint8_t flag
                , const float temperature
                , const float threshold
                , const float strength
                , const float radiation_ability
                , const uint32_t refractory_span
                , const uint32_t refractory_period);
  void SetParameter(const struct HydronParameter &parameter);
  struct HydronParameter Parameter() const {
    return parameter_;
  }

  ~Hydron();

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

  static common3d::BlockGrid NeighborSearcher() {
    return neighbor_hydron_searcher_;
  }

  static struct HydronParameter GetSpecifiedHydronParameter(HydronId id) {
    if (all_hydron_map_.find(id) == all_hydron_map_.end()) {
      struct HydronParameter p;
      p.SetNotValid();
      return p;
    }
    return all_hydron_map_[id]->Parameter();
  }

 private:
  HydronId id_;
  struct HydronParameter parameter_;
  float temperature_buffer_;
  std::list<struct HydronConnection> connecting_hydrons_;
  static std::map<HydronId, Hydron *> all_hydron_map_;
  static common3d::BlockGrid neighbor_hydron_searcher_;
};

}  // namespace hydron

#endif  // INCLUDE_HYDRON_H_
