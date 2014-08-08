// Copyright 2014 Makoto Yano

#include <memory>

#include "./colony.h"
#include "./hydron.h"
#include "./learning_theory.h"

namespace hydron {

// Do nothing.
void KeepCurrent::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  parameter->food -= parameter->feed_capability;
}

Hydron KeepCurrent::CreateHydron(
                std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  return Hydron(parameter->min_area_vertix);
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
}

Hydron FeedLearning::CreateHydron(
                std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) {
  return Hydron(parameter->min_area_vertix);
}

}  // namespace hydron
