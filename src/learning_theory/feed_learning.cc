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

FeedLearning::FeeedLearning() {
    parameter_ = std::shared_ptr<LTParameter>(new FeedLearningParameter);
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) {
  parameter_.Feeding();
  parameter_->Consume(hydron_map->size());
  float surplus_food = parameter->Food() / hydron_map->size();
}

bool PossibleToCreateNewHydron(
            const std::shared_ptr<struct ColonyArea> &area) {
  float density = parameter_->Food() / area->volume;
  return density > parameter_->threshold_density;
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
  parameter_->Consume(parameter_->CreateHydronCost());

  float create_connection_energy = parameter_->CreateHydronCost();
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

/////////////////////////////////////////////////////////////////////////////
// Pramaeter class for FeedLearning.
/////////////////////////////////////////////////////////////////////////////

FeedLearningParameter::FeedLearningParameter()
: feed_capability_(10000.0f)
, food_(0.0f)
, create_hydron_cost_(100.0f)
, threshold_density_(1.0f)
{}

void FeedLearningParameter::Import(FILE *file) {
  fread(&feed_capability_
      , sizeof(feed_capability_)
      , 1
      , file);
  fread(&food_
      , sizeof(food_)
      , 1
      , file);
  fread(&create_hydron_cost_
      , sizeof(create_hydron_cost_)
      , 1
      , file);
  fread(&threshold_density_
      , sizeof(threshold_density_)
      , 1
      , file);
  printf("feed_cap = %f, food = %f, create_cost = %f, threshold = %f\n"
      , feed_capability_
      , food_
      , create_hydron_cost_
      , threshold_density_);
}

void FeedLearningParameter::Export(FILE *file) {
  auto ExportFloat = [&file](const float value) -> void {
    fwrite(&value, sizeof(value), 1, file);
  };
  ExportFloat(feed_capability_);
  ExportFloat(food_);
  ExportFloat(create_hydron_cost_);
  ExportFloat(threshold_density_);
}

void FeedLearningParameter::Feeding() {
  food_ += feed_capability_;
}

void FeedLearningParameter::Consume(const float &meal) {
  food_ -= meal;
}

}  // namespace hydron
