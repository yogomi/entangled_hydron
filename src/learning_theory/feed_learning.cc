// Copyright 2014 Makoto Yano

#include <boost/optional.hpp>
#include <memory>
#include <functional>

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

FeedLearning::FeedLearning() {
  energy_parameter_.feed_capability = 10000.0f;
  energy_parameter_.food = 0.0f;
  energy_parameter_.create_hydron_cost = 100.0f;
  energy_parameter_.density_threshold = 1.0f;
}

void FeedLearning::ImportParameter(FILE *file) {
  fread(&energy_parameter_, sizeof(energy_parameter_), 1, file);
  ShowParameter();
}

void FeedLearning::ShowParameter() {
  printf("feed_cap = %f, food = %f, create_cost = %f, threshold = %f\n"
      , energy_parameter_.feed_capability
      , energy_parameter_.food
      , energy_parameter_.create_hydron_cost
      , energy_parameter_.density_threshold);
}

void FeedLearning::ExportParameter(FILE *file) {
  fwrite(&energy_parameter_, sizeof(energy_parameter_), 1, file);
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) {
  energy_parameter_.food += energy_parameter_.feed_capability;
  float food_for_any_hydron = energy_parameter_.food / hydron_map->size();

  energy_parameter_.food -= hydron_map->size();
}

int64_t FeedLearning::BornOrDeath(
            const std::shared_ptr<struct ColonyArea> &area) {
  float surplus_food = energy_parameter_.food -
                (area->volume * energy_parameter_.density_threshold);
  int64_t surplus =
      static_cast<int64_t>(surplus_food / energy_parameter_.create_hydron_cost);
  return surplus;
}

Hydron FeedLearning::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , const std::shared_ptr<struct ColonyArea> &area
              , std::shared_ptr<common3d::BlockGrid> &hydron_searcher) {  // NOLINT
  Hydron h = Hydron(Random<float>(area->min_area_vertix.x()
                                , area->max_area_vertix.x())
              , Random<float>(area->min_area_vertix.y()
                            , area->max_area_vertix.y())
              , Random<float>(area->min_area_vertix.z()
                            , area->max_area_vertix.z()));
  HydronId self_id = h.Id();
  energy_parameter_.food -= energy_parameter_.create_hydron_cost;

  float create_connection_energy = energy_parameter_.create_hydron_cost;
  boost::optional<float> create_connection_cost;

  common3d::NeighborhoodMap distance_map_in_colony =
                            hydron_searcher->GetNeighborsDistanceMap(self_id);
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
      for (const Eigen::Vector3f &hydron_position : DistanceInfo.second) {
        HydronId id(hydron_position);
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
    return boost::optional<float>((*id - hydron.Id()).norm());
  }

  return boost::none;
}

}  // namespace hydron
