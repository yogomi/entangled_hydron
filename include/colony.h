// Copyright 2014 Makoto Yano

#ifndef INCLUDE_COLONY_H_
#define INCLUDE_COLONY_H_

#include <map>
#include <string>

#include "./hydron.h"

namespace hydron {

class Colony {
 public:
  explicit Colony(const char *colony_name):colony_name_(colony_name) {}
  virtual ~Colony() {}

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


  // Save all Hydrons to file.
  // Filename is colony_name_ + .bin.
  void Save() const;

  // Clear all Hydrons and read file and set new Hydrons.
  // Filename is colony_name_ + .bin.
  // If could not open file, remain empty Hydron list.
  void Load();

  void Print() const;

 private:
  Colony() {}
  void Ignition_();
  void CalculateHeatEffect_();
  void ApplyFeedback_();
  int32_t ReadHydron_(FILE *file);
  std::string FileName_() const;

  std::string colony_name_;
  std::map<HydronId, Hydron> hydron_map_;
  static std::map<HydronId, Colony *> colony_signpost_;
  static std::map<HydronId, HydronIdList> connection_reverse_map_;
};

}  // namespace hydron

#endif  // INCLUDE_COLONY_H_
