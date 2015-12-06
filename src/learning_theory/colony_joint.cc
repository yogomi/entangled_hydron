// Copyright 2014 Makoto Yano

#include <Eigen/Core>
#include <boost/optional.hpp>
#include <memory>
#include <functional>

#include "./neighborhood_map.h"
#include "./colony.h"
#include "./hydron.h"
#include "./random_number_generator.h"
#include "learning_theory/learning_theory.h"
#include "learning_theory/colony_joint.h"

namespace hydron {

/////////////////////////////////////////////////////////////////////////////
// For connecting colony learning theory
/////////////////////////////////////////////////////////////////////////////

void ColonyJoint::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) {
}

int64_t ColonyJoint::BornOrDeath(
            const std::shared_ptr<struct ColonyArea> &area) {
  return 0;
}

Hydron ColonyJoint::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , const std::shared_ptr<struct ColonyArea> &area) {
  Hydron h = Hydron(Random<float>(area->min_area_vertix.x()
                                , area->max_area_vertix.x())
              , Random<float>(area->min_area_vertix.y()
                            , area->max_area_vertix.y())
              , Random<float>(area->min_area_vertix.z()
                            , area->max_area_vertix.z()));
  return h;
}

boost::optional<float> ColonyJoint::CreateConnection_(Hydron &hydron
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
