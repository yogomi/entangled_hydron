// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <boost/optional.hpp>
#include <cstdint>
#include <list>
#include <map>

#include "./vector.h"
#include "./neighborhood_map.h"

namespace hydron {

typedef common3d::Vector HydronId;
typedef std::list<HydronId> HydronIdList;

struct HydronParameter {
  float temperature;
  float threshold;
  float strength;
  float radiation_ability;
  uint32_t refractory_span;
  uint32_t refractory_period;
};

typedef boost::optional<struct HydronParameter> MaybeHydronParameter;
typedef std::map<HydronId, float> HydronConnections;

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

  void SetParameter(const float temperature
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

  HydronConnections ConnectingHydrons() const;

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

  static MaybeHydronParameter GetSpecifiedHydronParameter(const HydronId &id) {
    auto it = all_hydron_map_.find(id);
    if (it == all_hydron_map_.end()) {
      return boost::none;
    }
    return it->second->Parameter();
  }

 private:
  HydronId id_;
  struct HydronParameter parameter_;
  float temperature_buffer_;
  HydronConnections connecting_hydrons_;
  static std::map<HydronId, Hydron *> all_hydron_map_;
  static common3d::BlockGrid neighbor_hydron_searcher_;
};

}  // namespace hydron

#endif  // INCLUDE_HYDRON_H_
