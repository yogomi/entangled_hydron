// Copyright 2014 Makoto Yano

#ifndef INCLUDE_LEARNING_THEORY_LEARNING_THEORY_H_
#define INCLUDE_LEARNING_THEORY_LEARNING_THEORY_H_

#include <boost/optional.hpp>
#include <memory>
#include <map>

#include "./hydron.h"
#include "./colony.h"

namespace hydron {

enum LTType {
  KEEP_CURRENT = 0
  , COLONY_JOINT = 1
  , FEED_LEARNING = 2
};

class LTParameter {
 public:
  LTParameter() {}
  virtual ~LTParameter() {}
  virtual void Import(FILE *file) {}
  virtual void Export(FILE *file) {}
};

class LearningTheory {
 public:
  LearningTheory() {
    parameter = std::shared_ptr<LTParameter>(new LTParameter);
  }
  virtual ~LearningTheory() {}

  virtual enum LTType LTType() = 0;
  virtual void ImportParameter(FILE *file) {
    parameter->Import(file);
  }
  virtual void ExportParameter(FILE *file) {
    parameter->Export(file);
  }
  virtual void Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) = 0;
  virtual Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , std::shared_ptr<struct ColonyParameter> parameter) = 0;
 protected:
  std::shared_ptr<LTParameter> parameter;
};

std::shared_ptr<LearningTheory> CreateLearningTheory(
                              const LTType &learning_theory_type);

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_LEARNING_THEORY_H_
