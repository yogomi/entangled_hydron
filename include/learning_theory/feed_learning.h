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

struct FLParameter {
  float feed_capability;
  float food;
  float create_hydron_cost;
  float density_threshold;
};

class FeedLearning: public LearningTheory {
 public:
  FeedLearning();
  ~FeedLearning() {}

  virtual enum LTType LTType() {
    return FEED_LEARNING;
  }
  void ImportParameter(FILE *file);
  void ExportParameter(FILE *file);
  void ShowParameter();
  void Learning(std::shared_ptr<std::map<HydronId, Hydron>> hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);
  int64_t BornOrDeath(const std::shared_ptr<struct ColonyArea> &area);
  Hydron CreateHydron(
                  const std::shared_ptr<std::map<HydronId, Hydron>> &hydron_map
                , const std::shared_ptr<struct ColonyArea> &area);

 private:
  FLParameter fl_parameter_;
  boost::optional<float> CreateConnection_(Hydron &hydron
              , const common3d::NeighborhoodMap &distance_map_in_colony);
};

}  // namespace hydron

#endif  // INCLUDE_LEARNING_THEORY_FEED_LEARNING_H_
