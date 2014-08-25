// Copyright 2014 Makoto Yano

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
  } while (create_connection_energy > 0 && create_connection_cost != 0);
  return h;
}

float FeedLearning::CreateConnection_(const Hydron &hydron
            , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map) {
  common3d::BlockGrid neighbor_searcher = hydron.NeighborSearcher();
  common3d::NeighborhoodMap neighbor_map =
              neighbor_searcher.GetNeighborsDistanceMap(hydron.Id());

  if (neighbor_map.size() > 0) {
    return 0.0f;
  } else {
    common3d::NeighborhoodMap distance_map_in_colony;
    common3d::Vector id = hydron.Id();
    for (auto &h_info : *hydron_map) {
      distance_map_in_colony[id.DistanceTo(h_info.first)].push_back(
                                                            h_info.first);
    }
    return 0.0f;
  }
}

}  // namespace hydron
