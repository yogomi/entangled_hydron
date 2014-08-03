// Copyright 2014 Makoto Yano

#include "./learning_theory.h"

namespace hydron {

void KeepCurrent::Learning(std::map<HydronId, Hydron> &hydron_map
              , struct LearningParameter &parameter) {  // NOLINT
  parameter.food -= hydron_map.size();
}

void FeedLearning::Learning(std::map<HydronId, Hydron> &hydron_map
              , struct LearningParameter &parameter) {  // NOLINT
}

}  // namespace hydron
