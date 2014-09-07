// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_FEED_LEARNING_H_
#define INCLUDE_LEARNING_THEORY_FEED_LEARNING_H_

#include <boost/optional.hpp>
#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"
#include "learning_theory/learning_theory.h"

namespace hydron {

class FeedLearning: public LearningTheory {
 public:
  FeedLearning() {}
  ~FeedLearning() {}

  virtual enum LTType LTType() {
    return FEED_LEARNING;
  }
  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);

 private:
  boost::optional<float> CreateConnection_(Hydron &hydron
              , const common3d::NeighborhoodMap &distance_map_in_colony);
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_FEED_LEARNING_H_
