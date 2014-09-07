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
#include "learning_theory/learning_theory.h"
#include "learning_theory/colony_joint.h"

namespace hydron {

/////////////////////////////////////////////////////////////////////////////
// ColonyJoint learning theory
// It use for connecting nearby colony.
/////////////////////////////////////////////////////////////////////////////

void ColonyJoint::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= hydron_map->size();
  float surplus_food = parameter->food / hydron_map->size();
}

Hydron ColonyJoint::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , std::shared_ptr<struct ColonyParameter> parameter) {
  Hydron h = Hydron(Random<float>(parameter->min_area_vertix.x()
                                , parameter->max_area_vertix.x())
              , Random<float>(parameter->min_area_vertix.y()
                            , parameter->max_area_vertix.y())
              , Random<float>(parameter->min_area_vertix.z()
                            , parameter->max_area_vertix.z()));
  HydronId self_id = h.Id();
  parameter->food -= parameter->create_hydron_cost;

  float create_connection_energy = parameter->create_hydron_cost;
  boost::optional<float> create_connection_cost;

  common3d::NeighborhoodMap distance_map_in_colony;
  for (auto const &hydron_info : *hydron_map) {
    distance_map_in_colony[self_id.DistanceTo(hydron_info.first)].push_back(
                                                      hydron_info.first);
  }
  while (create_connection_energy > 0.0f) {
    create_connection_cost = CreateConnection_(h, distance_map_in_colony);
    if (!create_connection_cost) break;
    create_connection_energy -= *create_connection_cost;
  }
  return h;
}

boost::optional<float> ColonyJoint::CreateConnection_(Hydron &hydron
            , const common3d::NeighborhoodMap &distance_map_in_colony) {
  return boost::none;
}

}  // namespace hydron
