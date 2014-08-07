// Copyright 2014 Makoto Yano

#include "./learning_theory.h"

namespace hydron {

void KeepCurrent::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct LearningParameter> parameter) {
  parameter->food -= hydron_map->size();
}

void FeedLearning::Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct LearningParameter> parameter) {
}

}  // namespace hydron
