// Copyright 2014 Makoto Yano

#include <memory>

#include "./colony.h"
#include "./hydron.h"
#include "./random_number_generator.h"
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
  Hydron h = Hydron(Random<float>(parameter->min_area_vertix.x()
                                , parameter->max_area_vertix.x())
              , Random<float>(parameter->min_area_vertix.y()
                            , parameter->max_area_vertix.y())
              , Random<float>(parameter->min_area_vertix.z()
                            , parameter->max_area_vertix.z()));
  return h;
}

}  // namespace hydron
