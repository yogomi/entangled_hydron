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
#include "learning_theory/feed_learning.h"

namespace hydron {

/////////////////////////////////////////////////////////////////////////////
// FeedLearning learning theory
/////////////////////////////////////////////////////////////////////////////

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter_.Feeding();
  parameter_->food -= hydron_map->size();
  float surplus_food = parameter->food / hydron_map->size();
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

boost::optional<float> FeedLearning::CreateConnection_(Hydron &hydron
            , const common3d::NeighborhoodMap &distance_map_in_colony) {
  auto ConnectableHydronIdInNeighborMap = [&hydron](
      const common3d::NeighborhoodMap &neighbors) -> boost::optional<HydronId> {
    HydronConnections connections = hydron.ConnectingHydrons();
    for (auto &DistanceInfo : neighbors) {
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
  std::map<HydronId, Hydron>::iterator iter;
  // Find cold connectable hydron.
  boost::optional<HydronId> id
    = ConnectableHydronIdInNeighborMap(distance_map_in_colony);

  if (id) {
    hydron.ConnectTo(*id);
    return boost::optional<float>(id->DistanceTo(hydron.Id()));
  }

  return boost::none;
}

void FeedLearningParameter::Import(FILE *file) {
  fread(&feed_capability
      , sizeof(feed_capability)
      , 1
      , file);
  fread(&food_
      , sizeof(food)
      , 1
      , file);
  fread(&create_hydron_cost
      , sizeof(create_hydron_cost)
      , 1
      , file);
  fread(&threshold_density
      , sizeof(threshold_density)
      , 1
      , file);
  printf("feed_cap = %f, food = %f, create_cost = %f, threshold = %f\n"
      , feed_capability
      , food
      , create_hydron_cost
      , threshold_density);
}

void FeedLearningParameter::Export(FILE *file) {
  auto ExportFloat = [&file](const float value) -> void {
    fwrite(&value, sizeof(value), 1, file);
  };
  ExportFloat(feed_capability);
  ExportFloat(food);
  ExportFloat(create_hydron_cost);
  ExportFloat(threshold_density);
}

void FeedLearningParameter::Feeding() {
  food += feed_capability;
}

}  // namespace hydron
