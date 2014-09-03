// Copyright 2014 Makoto Yano

#include <memory>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "x64_x32/format_type.h"
#include "./vector.h"
#include "./cube.h"
#include "./hydron.h"
#include "./colony.h"

namespace hydron {

struct FileDeleter {
  void operator()(FILE *ptr) const {
    std::fclose(ptr);
  }
};

Colony::~Colony() {
  for (auto &h : *hydron_map_) {
    colony_signpost_.erase(h.first);
    connection_reverse_map_.erase(h.first);
    for (auto &reverse_info : connection_reverse_map_) {
      for_each(reverse_info.second.begin()
            , reverse_info.second.end()
            , [&reverse_info, &h](HydronId id) {
            if (h.first == id) {
              reverse_info.second.remove(id);
            }
          });
    }
  }
}

void Colony::SetLearningTheory(
    std::shared_ptr<LearningTheory> learning_theory) {
  learning_theory_ = learning_theory;
}

void Colony::Ignition() {
  for (auto& h : *hydron_map_) {
    h.second.Fire();
  }
}

void Colony::CalculateHeatEffect() {
  for (auto& h : *hydron_map_) {
    h.second.AdaptHeatEffect();
  }
}

void Colony::ApplyFeedback() {
  Feeding_();
  learning_theory_->Learning(hydron_map_, parameter_);
  Digest_();
}

int32_t Colony::AddHydron(const Hydron &hydron) {
  // If same ID hydron exist in any colony.
  // Fail AddHydron.
  if (colony_signpost_.find(hydron.Id()) != colony_signpost_.end()) {
    return -1;
  }
  (*hydron_map_)[hydron.Id()] = hydron;
  colony_signpost_[hydron.Id()] = this;
  (*hydron_map_)[hydron.Id()].RegisterToAllHydronMap();

  HydronConnections connecting_hydrons = hydron.ConnectingHydrons();
  for (const auto &connection : connecting_hydrons) {
    connection_reverse_map_[connection.first].push_back(hydron.Id());
  }

  return 0;
}

void Colony::ConnectHydronToHydron(const Hydron &from
                                , const Hydron &to
                                , const float weight) {
  ConnectHydronToHydron(from.Id(), to.Id(), weight);
}

void Colony::ConnectHydronToHydron(const HydronId &from
                                , const HydronId &to
                                , const float weight) {
  if (hydron_map_->find(from) != hydron_map_->end()) {
    (*hydron_map_)[from].ConnectTo(to, weight);
    connection_reverse_map_[to].push_back(from);
  }
}

void Colony::SetParameter(
    const struct ColonyParameter &parameter) {
  *parameter_ = parameter;
}

void Colony::Save() const {
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "wb"));

  uint64_t header[2] = {0, 0};
  fwrite(header, sizeof(uint64_t), 2, file.get());
  ExportParameter_(file.get());
  for (const auto& h : *hydron_map_) {
    h.second.ExportStatus(file.get());
  }
}

void Colony::Load() {
  hydron_map_->clear();
  connection_reverse_map_.clear();
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "rb"));
  if (file.get() == nullptr) {
    return;
  }
  uint64_t header[2];
  fread(header, sizeof(uint64_t), 2, file.get());
  ImportParameter_(file.get());

  printf("feed_cap = %f, food = %f, create_cost = %f, threshold = %f\n"
      , parameter_->feed_capability
      , parameter_->food
      , parameter_->create_hydron_cost
      , parameter_->threshold_density);
  while (ReadHydron_(file.get()) == 0) {}
}

Colony* Colony::GetAffiliatedColony(const HydronId id) {
  if (colony_signpost_.find(id) == colony_signpost_.end()) {
    return nullptr;
  }
  return colony_signpost_[id];
}

void Colony::ShowHydronsStatus() const {
  for (auto& h : *hydron_map_) {
    h.second.ShowStatus();
  }
  printf("hydron amount = %" PRIuS "\n", hydron_map_->size());
}

void Colony::ShowSignpostInformation() const {
  printf("=================");
  printf("ShowSignpostInformation");
  printf(" Begin");
  printf("=================\n");
  for (auto& info : colony_signpost_) {
    printf("((%f, %f, %f), %s)\n"
        , info.first.x()
        , info.first.y()
        , info.first.z()
        , info.second->Name().c_str());
  }
  printf("=================");
  printf("ShowSignpostInformation");
  printf(" End");
  printf("=================\n");
}

void Colony::ShowConnectionReverseMap() const {
  printf("=================");
  printf("ShowConnectionReverseMap");
  printf(" Begin");
  printf("=================\n");
  for (auto& connection : connection_reverse_map_) {
    printf("From:(%f, %f, %f) To:"
        , connection.first.x()
        , connection.first.y()
        , connection.first.z());
    for_each(connection.second.begin()
            , connection.second.end()
            , [](const HydronId &id) {
      printf("(%f, %f, %f), ", id.x(), id.y(), id.z());
    });
    printf("\n");
  }
  printf("=================");
  printf("ShowConnectionReverseMap");
  printf(" End");
  printf("=================\n");
}

void Colony::Feeding_() {
  parameter_->food += parameter_->feed_capability;
}

void Colony::Digest_() {
  float volume = common3d::CubeVolumeFromBothEndVertix(
                        parameter_->max_area_vertix
                        , parameter_->min_area_vertix);
  float density = parameter_->food / volume;
  if (density > parameter_->threshold_density) {
    printf("density = %f\n", density);
    Hydron h = learning_theory_->CreateHydron(hydron_map_, parameter_);
    AddHydron(h);
  }
}

void Colony::ExportParameter_(FILE *file) const {
  auto ExportFloat = [&file](const float value) -> void {
    fwrite(&value, sizeof(value), 1, file);
  };
  auto ExportVector = [&file, &ExportFloat](const common3d::Vector &v) -> void {
    ExportFloat(v.x());
    ExportFloat(v.y());
    ExportFloat(v.z());
  };
  ExportVector(parameter_->max_area_vertix);
  ExportVector(parameter_->min_area_vertix);
  ExportFloat(parameter_->feed_capability);
  ExportFloat(parameter_->food);
  ExportFloat(parameter_->create_hydron_cost);
  ExportFloat(parameter_->threshold_density);
}

void Colony::ImportParameter_(FILE *file) {
  struct common3d::MinimumElementVector v;
  fread(&v, sizeof(v), 1, file);
  parameter_->max_area_vertix = common3d::Vector(v.x, v.y, v.z);
  fread(&v, sizeof(v), 1, file);
  parameter_->min_area_vertix = common3d::Vector(v.x, v.y, v.z);
  fread(&(parameter_->feed_capability)
      , sizeof(parameter_->feed_capability)
      , 1
      , file);
  fread(&(parameter_->food)
      , sizeof(parameter_->food)
      , 1
      , file);
  fread(&(parameter_->create_hydron_cost)
      , sizeof(parameter_->create_hydron_cost)
      , 1
      , file);
  fread(&(parameter_->threshold_density)
      , sizeof(parameter_->threshold_density)
      , 1
      , file);
}

int32_t Colony::ReadHydron_(FILE *file) {
  struct common3d::MinimumElementVector v;
  struct HydronParameter hydron_parameter;
  uint64_t connecting_hydron_count;

  if (fread(&v, sizeof(v), 1, file) <= 0) {
    return -1;
  }
  Hydron h(v.x, v.y, v.z);

  fread(&hydron_parameter, sizeof(hydron_parameter), 1, file);
  h.SetParameter(hydron_parameter);

  AddHydron(h);

  fread(&connecting_hydron_count, sizeof(connecting_hydron_count), 1, file);
  for (uint64_t i = 0; i < connecting_hydron_count; ++i) {
    float weight;
    fread(&v, sizeof(v), 1, file);
    fread(&weight, sizeof(weight), 1, file);
    ConnectHydronToHydron(h.Id(), HydronId(v.x, v.y, v.z), weight);
  }

  return 0;
}

std::string Colony::FileName_() const {
  return colony_name_ + ".bin";
}

std::map<HydronId, Colony *> Colony::colony_signpost_;
std::map<HydronId, HydronIdList> Colony::connection_reverse_map_;

void AlliedColonies::SetColony(const std::shared_ptr<Colony> &colony) {
  colony_map_[colony->Name()] = colony;
}

void AlliedColonies::Beat() {
  IgnitionAllColonies_();
  CalculateAllColoniesHeatEffect_();
  ApplyAllColoniesFeedback_();
}

void AlliedColonies::IgnitionAllColonies_() {
  for (auto &colony : colony_map_) {
    colony.second->Ignition();
  }
}

void AlliedColonies::CalculateAllColoniesHeatEffect_() {
  for (auto &colony : colony_map_) {
    colony.second->CalculateHeatEffect();
  }
}

void AlliedColonies::ApplyAllColoniesFeedback_() {
  for (auto &colony : colony_map_) {
    colony.second->ApplyFeedback();
  }
}

}  // namespace hydron
