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
                , const std::shared_ptr<struct ColonyParameter> &parameter) {
  return Hydron(parameter->min_area_vertix);
}

HydronId KeepCurrent::FindEasyToConnectHydron(const Hydron &hydron
            , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map) {
  return hydron.Id();
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
}

Hydron FeedLearning::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , const std::shared_ptr<struct ColonyParameter> &parameter) {
  Hydron h = Hydron(Random<float>(parameter->min_area_vertix.x()
                                , parameter->max_area_vertix.x())
              , Random<float>(parameter->min_area_vertix.y()
                            , parameter->max_area_vertix.y())
              , Random<float>(parameter->min_area_vertix.z()
                            , parameter->max_area_vertix.z()));
  return h;
}

HydronId FeedLearning::FindEasyToConnectHydron(const Hydron &hydron
            , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map) {
  common3d::BlockGrid neighbor_searcher = hydron.NeighborSearcher();
  common3d::NeighborhoodMap neighbor_map =
              neighbor_searcher.GetNeighborsDistanceMap(hydron.Id());

  std::function<bool(const common3d::Vector &)> ScaleFromRoundSlice =
                      ScaleOfUnitBVectorFromPlanePathAVerticallyB(
                                  hydron.Id(), hydron.HeadDirection());
  printf("wowwwwwww!!_\n");
  if (neighbor_map.size() > 0) {
    return hydron.Id();
  } else {
    common3d::NeighborhoodMap distance_map_in_colony;
    common3d::Vector id = hydron.Id();
    for (auto &h_info : *hydron_map) {
      distance_map_in_colony[id.DistanceTo(h_info.first)].push_back(
                                                            h_info.first);
    }
    return hydron.Id();
  }
}

}  // namespace hydron
