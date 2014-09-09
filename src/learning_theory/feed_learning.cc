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

FeedLearning::FeedLearning() {
  fl_parameter_.feed_capability = 10000.0f;
  fl_parameter_.food = 0.0f;
  fl_parameter_.create_hydron_cost = 100.0f;
  fl_parameter_.density_threshold = 1.0f;
}

void FeedLearning::ImportParameter(FILE *file) {
  fread(&fl_parameter_, sizeof(fl_parameter_), 1, file);
  printf("feed_cap = %f, food = %f, create_cost = %f, threshold = %f\n"
      , fl_parameter_.feed_capability
      , fl_parameter_.food
      , fl_parameter_.create_hydron_cost
      , fl_parameter_.density_threshold);
}

void FeedLearning::ExportParameter(FILE *file) {
  fwrite(&fl_parameter_, sizeof(fl_parameter_), 1, file);
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) {
  fl_parameter_.food += fl_parameter_.feed_capability;
  fl_parameter_.food -= hydron_map->size();
  float surplus_food = fl_parameter_.food / hydron_map->size();
}

bool FeedLearning::PossibleToCreateNewHydron(
            const std::shared_ptr<struct ColonyArea> &area) {
  float density = fl_parameter_.food / area->volume;
  return density > fl_parameter_.density_threshold;
}

Hydron FeedLearning::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , const std::shared_ptr<struct ColonyArea> &area) {
  Hydron h = Hydron(Random<float>(area->min_area_vertix.x()
                                , area->max_area_vertix.x())
              , Random<float>(area->min_area_vertix.y()
                            , area->max_area_vertix.y())
              , Random<float>(area->min_area_vertix.z()
                            , area->max_area_vertix.z()));
  HydronId self_id = h.Id();
  fl_parameter_.food -= fl_parameter_.create_hydron_cost;

  float create_connection_energy = fl_parameter_.create_hydron_cost;
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

}  // namespace hydron
