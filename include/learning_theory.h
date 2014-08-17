// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_H_
#define INCLUDE_LEARNING_THEORY_H_

#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"

namespace hydron {

class LearningTheory {
 public:
  LearningTheory() {}
  ~LearningTheory() {}

  virtual void Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) = 0;
  virtual Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyParameter> &parameter) = 0;
};

class KeepCurrent: public LearningTheory {
 public:
  KeepCurrent() {}
  ~KeepCurrent() {}

  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyParameter> &parameter);
};

class FeedLearning: public LearningTheory {
 public:
  FeedLearning() {}
  ~FeedLearning() {}

  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyParameter> &parameter);

 private:
  HydronId CreateConnection_(const Hydron &hydron
              , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map);
  HydronId CreateReverseConnection_(const Hydron &hydron
              , const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map);
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_H_
