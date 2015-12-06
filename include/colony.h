// Copyright 2014 Makoto Yano

#ifndef INCLUDE_COLONY_H_
#define INCLUDE_COLONY_H_

#include <Eigen/Core>

#include <memory>
#include <list>
#include <map>
#include <string>

#include "./hydron.h"
#include "learning_theory/learning_theory.h"
#include "learning_theory/keep_current.h"

namespace hydron {

struct ColonyArea {
  Eigen::Vector3f max_area_vertix;
  Eigen::Vector3f min_area_vertix;
  float volume;
};

class Colony {
 public:
  explicit Colony(const char *colony_name)
    :colony_name_(colony_name)
    , area_(new struct ColonyArea)
    , learning_theory_(new KeepCurrent)
    , hydron_map_(new std::map<HydronId, Hydron>)
    , idling_hydron_ranking_(new std::list<HydronId>) {
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
  void DeleteHydron(const HydronId &id);
  void ConnectHydronToHydron(const Hydron &from
                                , const Hydron &to
                                , const float weight = 1);
  void ConnectHydronToHydron(const HydronId &from
                                , const HydronId &to
                                , const float weight = 1);

  struct ColonyArea Area() {
    return *area_;
  }
  void SetArea(const Eigen::Vector3f &max, const Eigen::Vector3f &min);

  // Save all Hydrons to file.
  // Filename is colony_name_ + .bin.
  void Save() const;

  // Clear all Hydrons and read file and set new Hydrons.
  // Filename is colony_name_ + .bin.
  // If it could not open the file, remain empty Hydron list.
  void Load();

  void ShowHydronsStatus() const;
  void ShowConnectionReverseMap() const;

  std::string Name() const {
    return colony_name_;
  }

 private:
  Colony() {}

  void Initialize_();
  void Digest_();

  void ExportParameter_(FILE *file) const;
  void ImportParameter_(FILE *file);
  int32_t ReadHydron_(FILE *file);
  std::string FileName_() const;

  std::shared_ptr<struct ColonyArea> area_;
  std::shared_ptr<LearningTheory> learning_theory_;

  std::string colony_name_;
  std::shared_ptr<std::map<HydronId, Hydron>> hydron_map_;
  std::shared_ptr<std::list<HydronId>> idling_hydron_ranking_;
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
