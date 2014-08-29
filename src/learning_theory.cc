// Copyright 2014 Makoto Yano

#include <boost/optional.hpp>
#include <memory>
#include <functional>

#include "./vector.h"
#include "./math3d.h"
#include "./neighborhood_map.h"
#include "./colony.h"
#include "./hydron.h"
#include "./random_number_generator.h"
#include "./learning_theory.h"

namespace hydron {

/////////////////////////////////////////////////////////////////////////////
// KeepCurrent learning theory
// Do nothing.
/////////////////////////////////////////////////////////////////////////////

void KeepCurrent::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= parameter->feed_capability;
}

Hydron KeepCurrent::CreateHydron(
                const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= parameter->create_hydron_cost;
  return Hydron(parameter->min_area_vertix);
}

/////////////////////////////////////////////////////////////////////////////
// FeedLearning learning theory
/////////////////////////////////////////////////////////////////////////////

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
}

Hydron FeedLearning::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , std::shared_ptr<struct ColonyParameter> parameter) {
  Hydron h = Hydron(Random<float>(parameter->min_area_vertix.x()
                                , parameter->max_area_vertix.x())
              , Random<float>(parameter->min_area_vertix.y()
                            , parameter->max_area_vertix.y())
              , Random<float>(parameter->min_area_vertix.z()
                            , parameter->max_area_vertix.z()));
  parameter->food -= parameter->create_hydron_cost;

  float create_connection_energy = parameter->create_hydron_cost;
  float create_connection_cost;

  do {
    create_connection_cost = CreateConnection_(h, hydron_map);
    create_connection_energy -= create_connection_cost;
  } while (create_connection_energy > 0.0f && create_connection_cost != 0.0f);
  return h;
}

float FeedLearning::CreateConnection_(Hydron &hydron
            , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map) {
  common3d::BlockGrid neighbor_searcher = hydron.NeighborSearcher();
  common3d::NeighborhoodMap neighbor_map =
              neighbor_searcher.GetNeighborsDistanceMap(hydron.Id());


  auto ConnectableHydronIdInNeighborMap = [&hydron](
      const common3d::NeighborhoodMap &neighbors) -> boost::optional<HydronId> {
    HydronConnections connections = hydron.ConnectingHydrons();
    for (auto DistanceInfo : neighbors) {
      for (const HydronId &id : DistanceInfo.second) {
        if (connections.find(id) != connections.end()) continue;
        if (id == hydron.Id()) continue;
        MaybeHydronParameter param = Hydron::GetSpecifiedHydronParameter(id);
        if (param && param->temperature < (param->threshold / 2)) {
          return id;
        }
      }
    }
    return boost::none;
  };

  // Find neaby cold connectable hydron.
  boost::optional<HydronId> id = ConnectableHydronIdInNeighborMap(neighbor_map);
  if (id) {
    hydron.ConnectTo(*id);
    return id->DistanceTo(hydron.Id());
  }

  // Create far distance map.
  common3d::NeighborhoodMap distance_map_in_colony;
  HydronId self_id = hydron.Id();
  std::map<HydronId, Hydron>::iterator iter;
  for (int32_t i = 0; i < 10000; i++) {
    iter = hydron_map->begin();
    std::advance(iter, Random<uint64_t>(0, hydron_map->size()));
    distance_map_in_colony[self_id.DistanceTo(iter->first)].push_back(
                                                        iter->first);
  }
  // Find far cold connectable hydron.
  id = ConnectableHydronIdInNeighborMap(distance_map_in_colony);
  if (id) {
    hydron.ConnectTo(*id);
    return id->DistanceTo(hydron.Id());
  }

  return 0.0f;
}

}  // namespace hydron
