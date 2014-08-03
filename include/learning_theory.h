// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_H_
#define INCLUDE_LEARNING_THEORY_H_

#include <map>

#include "./hydron.h"

namespace hydron {

struct LearningParameter {
  float food;
};

class LearningTheory {
 public:
  LearningTheory() {}
  ~LearningTheory() {}

  virtual void Learning(std::map<HydronId, Hydron> &hydron_map
                      , struct LearningParameter &parameter) = 0;  // NOLINT
};

class KeepCurrent: public LearningTheory {
 public:
  KeepCurrent() {}
  ~KeepCurrent() { printf("KeepCurrent Destructor\n"); }

  virtual void Learning(std::map<HydronId, Hydron> &hydron_map
                      , struct LearningParameter &parameter);  // NOLINT
};

class FeedLearning: public LearningTheory {
 public:
  FeedLearning() {}
  ~FeedLearning() {}

  void Learning(std::map<HydronId, Hydron> &hydron_map
              , struct LearningParameter &parameter);  // NOLINT
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_H_
