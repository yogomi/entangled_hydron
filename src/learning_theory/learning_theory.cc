// Copyright 2014 Makoto Yano

#include <boost/optional.hpp>
#include <memory>
#include <functional>

#include "./vector.h"
#include "./math3d.h"
#include "./neighborhood_map.h"
#include "./colony.h"
#include "./hydron.h"
#include "./random_number_generator.h"
#include "learning_theory/learning_theory.h"
#include "learning_theory/keep_current.h"
#include "learning_theory/colony_joint.h"
#include "learning_theory/feed_learning.h"

namespace hydron {

std::shared_ptr<LearningTheory> CreateLearningTheory(
                          const LTType &learning_theory_type) {
  switch (learning_theory_type) {
    case KEEP_CURRENT:
      return std::shared_ptr<LearningTheory>(new KeepCurrent);
    case COLONY_JOINT:
      return std::shared_ptr<LearningTheory>(new ColonyJoint);
    case FEED_LEARNING:
      return std::shared_ptr<LearningTheory>(new FeedLearning);
    default:
      return nullptr;
  }
}

}  // namespace hydron
