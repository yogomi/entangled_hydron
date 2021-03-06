// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_KEEP_CURRENT_H_
#define INCLUDE_LEARNING_THEORY_KEEP_CURRENT_H_

#include <boost/optional.hpp>
#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"
#include "learning_theory/learning_theory.h"

namespace hydron {

class KeepCurrent: public LearningTheory {
 public:
  KeepCurrent() {}
  ~KeepCurrent() {}

  virtual enum LTType LTType() {
    return KEEP_CURRENT;
  }
  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);
  int64_t BornOrDeath(const std::shared_ptr<struct ColonyArea> &area);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyArea> &area
                , std::shared_ptr<common3d::BlockGrid> &hydron_searcher); // NOLINT
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_KEEP_CURRENT_H_
