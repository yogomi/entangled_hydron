// Copyright 2014 Makoto Yano

#ifndef INCLUDE_COLONY_H_
#define INCLUDE_COLONY_H_

#include <memory>
#include <map>
#include <string>

#include "./hydron.h"
#include "./learning_theory.h"

namespace hydron {

class Colony {
 public:
  explicit Colony(const char *colony_name)
    :colony_name_(colony_name)
    , feed_capability_(1.0f)
    , learning_theory_(new KeepCurrent) {}
  ~Colony();

  void SetLearningTheory(std::shared_ptr<LearningTheory> learning_theory);

  void Beat();

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

  void SetFeedCapability(const float &feed_capability);
  float FeedCapability() const {
    return feed_capability_;
  }

  // Save all Hydrons to file.
  // Filename is colony_name_ + .bin.
  void Save() const;

  // Clear all Hydrons and read file and set new Hydrons.
  // Filename is colony_name_ + .bin.
  // If could not open file, remain empty Hydron list.
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
  void Ignition_();
  void CalculateHeatEffect_();
  void ApplyFeedback_();
  int32_t ReadHydron_(FILE *file);
  std::string FileName_() const;

  float feed_capability_;
  std::shared_ptr<LearningTheory> learning_theory_;

  std::string colony_name_;
  std::map<HydronId, Hydron> hydron_map_;
  static std::map<HydronId, Colony *> colony_signpost_;
  static std::map<HydronId, HydronIdList> connection_reverse_map_;
};

}  // namespace hydron

#endif  // INCLUDE_COLONY_H_
