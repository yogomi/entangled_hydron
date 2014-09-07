// Copyright 2014 Makoto Yano

#ifndef INCLUDE_COLONY_H_
#define INCLUDE_COLONY_H_

#include <memory>
#include <map>
#include <string>

#include "./vector.h"
#include "./hydron.h"
#include "learning_theory/learning_theory.h"

namespace hydron {

struct ColonyParameter {
  common3d::Vector max_area_vertix;
  common3d::Vector min_area_vertix;
  float feed_capability;
  float food;
  float create_hydron_cost;
  float threshold_density;
};

class Colony {
 public:
  explicit Colony(const char *colony_name)
    :colony_name_(colony_name)
    , parameter_(new struct ColonyParameter)
    , learning_theory_(new KeepCurrent)
    , hydron_map_(new std::map<HydronId, Hydron>) {
      Initialize_();
    }
  ~Colony();

  void SetLearningTheory(const LTType &lt_type);

  // Ignition, CalculationHeatEffect and ApplyFeedback are
  // functions called by Beat function in AlliedColonies class.
  void Ignition();
  void CalculateHeatEffect();
  void ApplyFeedback();

  // Add hydron data and map to colony.
  // Normary success, return 0.
  // If failed, return a minus value.
  int32_t AddHydron(const Hydron &hydron);
  void ConnectHydronToHydron(const Hydron &from
                                , const Hydron &to
                                , const float weight = 1);
  void ConnectHydronToHydron(const HydronId &from
                                , const HydronId &to
                                , const float weight = 1);

  struct ColonyParameter Parameter() {
    return *parameter_;
  }
  void SetParameter(const struct ColonyParameter &parameter);

  // Save all Hydrons to file.
  // Filename is colony_name_ + .bin.
  void Save() const;

  // Clear all Hydrons and read file and set new Hydrons.
  // Filename is colony_name_ + .bin.
  // If it could not open the file, remain empty Hydron list.
  void Load();

  static Colony* GetAffiliatedColony(const HydronId id);

  void ShowHydronsStatus() const;
  void ShowSignpostInformation() const;
  void ShowConnectionReverseMap() const;

  std::string Name() const {
    return colony_name_;
  }

 private:
  Colony() {}

  void Initialize_() {
    parameter_->max_area_vertix = common3d::Vector(100.0f, 100.0f, 100.0f);
    parameter_->min_area_vertix = common3d::Vector(-100.0f, -100.0f, -100.0f);
    parameter_->feed_capability = 100000000.0f;
    parameter_->food = 0.0f;
    parameter_->create_hydron_cost = 1000.0f;
    parameter_->threshold_density = 10.0f;
  }
  void Feeding_();
  void Digest_();

  void ExportParameter_(FILE *file) const;
  void ImportParameter_(FILE *file);
  int32_t ReadHydron_(FILE *file);
  std::string FileName_() const;

  std::shared_ptr<struct ColonyParameter> parameter_;
  std::shared_ptr<LearningTheory> learning_theory_;

  std::string colony_name_;
  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map_;
  static std::map<HydronId, Colony *> colony_signpost_;
  static std::map<HydronId, HydronIdList> connection_reverse_map_;
};

class AlliedColonies {
 public:
  AlliedColonies() {}
  ~AlliedColonies() {}

  void SetColony(const std::shared_ptr<Colony> &colony);

  void Beat();

 private:
  void IgnitionAllColonies_();
  void CalculateAllColoniesHeatEffect_();
  void ApplyAllColoniesFeedback_();

  std::map<std::string, std::shared_ptr<Colony>> colony_map_;
};

}  // namespace hydron

#endif  // INCLUDE_COLONY_H_
