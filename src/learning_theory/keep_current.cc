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
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= parameter->feed_capability;
}

Hydron KeepCurrent::CreateHydron(
                const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= parameter->create_hydron_cost;
  return Hydron(parameter->min_area_vertix);
}

}  // namespace hydron
