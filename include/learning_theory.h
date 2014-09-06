// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_H_
#define INCLUDE_LEARNING_THEORY_H_

#include <boost/optional.hpp>
#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"

namespace hydron {

enum LTType {
  KEEP_CURRENT = 0
  , FEED_LEARNING = 1
};

class LearningTheory {
 public:
  LearningTheory() {}
  ~LearningTheory() {}

  virtual void Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) = 0;
  virtual Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) = 0;
};

class KeepCurrent: public LearningTheory {
 public:
  KeepCurrent() {}
  ~KeepCurrent() {}

  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);
};

std::shared_ptr<LearningTheory> CreateLearningTheory(
                              const LTType &learning_theory_type);

class FeedLearning: public LearningTheory {
 public:
  FeedLearning() {}
  ~FeedLearning() {}

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

#endif  // INCLUDE_LEARNING_THEORY_H_
