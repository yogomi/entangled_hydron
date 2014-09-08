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
  FeedLearning();
  ~FeedLearning() {}

  virtual enum LTType LTType() {
    return FEED_LEARNING;
  }
  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);
  bool PossibleToCreateNewHydron(
                  const std::shared_ptr<struct ColonyArea> &area);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyParameter> &area);

 private:
  boost::optional<float> CreateConnection_(Hydron &hydron
              , const common3d::NeighborhoodMap &distance_map_in_colony);
};

class FeedLearningParameter: public LTParameter {
 public:
  FeedLearningParameter();
  ~FeedLearningParameter() {}
  void Import(FILE *file);
  void Export(FILE *file);

  void Feeding();
  void Consume(const float &meal);
  float Food() {
    return food_;
  }
  float CreateHydronCost() {
    return create_hydron_cost_;
  }

 private:
  float feed_capability_;
  float food_;
  float create_hydron_cost_;
  float threshold_density_;
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_FEED_LEARNING_H_
