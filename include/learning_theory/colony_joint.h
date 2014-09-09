// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_COLONY_JOINT_H_
#define INCLUDE_LEARNING_THEORY_COLONY_JOINT_H_

#include <boost/optional.hpp>
#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"
#include "learning_theory/learning_theory.h"

namespace hydron {

class ColonyJoint: public LearningTheory {
 public:
  ColonyJoint() {}
  ~ColonyJoint() {}

  virtual enum LTType LTType() {
    return COLONY_JOINT;
  }
  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);
  int64_t BornOrDeath(const std::shared_ptr<struct ColonyArea> &area);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);
 private:
  boost::optional<float> CreateConnection_(Hydron &hydron
              , const common3d::NeighborhoodMap &distance_map_in_colony);
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_COLONY_JOINT_H_
