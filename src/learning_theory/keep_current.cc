// Copyright 2014 Makoto Yano

#include <memory>

#include "./colony.h"
#include "./hydron.h"
#include "learning_theory/learning_theory.h"
#include "learning_theory/keep_current.h"

namespace hydron {

/////////////////////////////////////////////////////////////////////////////
// KeepCurrent learning theory
// Do nothing.
/////////////////////////////////////////////////////////////////////////////

void KeepCurrent::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) {
}

int64_t KeepCurrent::BornOrDeath(
                const std::shared_ptr<struct ColonyArea> &area) {
  return false;
}

Hydron KeepCurrent::CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
              , const std::shared_ptr<struct ColonyArea> &area
              , std::shared_ptr<common3d::BlockGrid> &hydron_searcher) {  // NOLINT
  return Hydron(area->min_area_vertix);
}

}  // namespace hydron
