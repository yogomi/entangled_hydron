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

class LearningTheory {
 public:
  LearningTheory() {}
  virtual ~LearningTheory() {}

  virtual enum LTType LTType() = 0;
  virtual void ImportParameter(FILE *file) {}
  virtual void ExportParameter(FILE *file) {}
  virtual void ShowParameter() {}
  virtual void Learning(
                  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area) = 0;
  virtual int64_t BornOrDeath(
                  const std::shared_ptr<struct ColonyArea> &area) = 0;
  virtual Hydron CreateHydron(
              const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
            , const std::shared_ptr<struct ColonyArea> &area
            , std::shared_ptr<common3d::BlockGrid> &hydron_searcher) = 0; // NOLINT
};

std::shared_ptr<LearningTheory> CreateLearningTheory(
                              const LTType &learning_theory_type);

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_LEARNING_THEORY_H_
