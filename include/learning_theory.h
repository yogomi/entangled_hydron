// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_H_
#define INCLUDE_LEARNING_THEORY_H_

#include <memory>
#include <map>
#include <string>

#include "./hydron.h"

namespace hydron {

struct LearningParameter {
  float food;
};

class LearningTheory {
 public:
  LearningTheory() {}
  ~LearningTheory() {}

  virtual void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                  , std::shared_ptr<struct LearningParameter> parameter) = 0;
};

class KeepCurrent: public LearningTheory {
 public:
  KeepCurrent() {}
  ~KeepCurrent() { printf("KeepCurrent Destructor\n"); }

  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct LearningParameter> parameter);
};

class FeedLearning: public LearningTheory {
 public:
  FeedLearning() {}
  ~FeedLearning() {}

  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct LearningParameter> parameter);
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_H_
