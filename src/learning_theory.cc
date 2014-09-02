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
  parameter->food -= hydron_map->size();
  float surplus_food = parameter->food / hydron_map->size();
}

Hydron FeedLearning::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , std::shared_ptr<struct ColonyParameter> parameter) {
  printf("CreateHydron Start\n");
  Hydron h = Hydron(Random<float>(parameter->min_area_vertix.x()
                                , parameter->max_area_vertix.x())
              , Random<float>(parameter->min_area_vertix.y()
                            , parameter->max_area_vertix.y())
              , Random<float>(parameter->min_area_vertix.z()
                            , parameter->max_area_vertix.z()));
  parameter->food -= parameter->create_hydron_cost;

  float create_connection_energy = parameter->create_hydron_cost;
  boost::optional<float> create_connection_cost;

  while (create_connection_energy > 0.0f) {
    create_connection_cost = CreateConnection_(h, hydron_map);
    if (!create_connection_cost) break;
    create_connection_energy -= *create_connection_cost;
  }
  printf("CreateHydron End\n");
  return h;
}

boost::optional<float> FeedLearning::CreateConnection_(Hydron &hydron
            , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map) {
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

  // Create far distance map.
  common3d::NeighborhoodMap distance_map_in_colony;
  HydronId self_id = hydron.Id();
  std::map<HydronId, Hydron>::iterator iter;
  for (auto const &h : *hydron_map) {
    distance_map_in_colony[self_id.DistanceTo(h.first)].push_back(
                                                        h.first);
  }
  // Find cold connectable hydron.
  boost::optional<HydronId> id
    = ConnectableHydronIdInNeighborMap(distance_map_in_colony);

  if (id) {
    hydron.ConnectTo(*id);
    return id->DistanceTo(hydron.Id());
  }

  return boost::none;
}

}  // namespace hydron
