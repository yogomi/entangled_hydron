// Copyright 2014 Makoto Yano

#include <Eigen/Core>

#include <memory>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "x64_x32/format_type.h"
#include "./cube.h"
#include "./hydron.h"
#include "./colony.h"
#include "learning_theory/learning_theory.h"
#include "debug/log.h"

namespace hydron {

struct FileDeleter {
  void operator()(FILE *ptr) const {
    std::fclose(ptr);
  }
};

Colony::~Colony() {
  for (auto &h : *hydron_map_) {
    connection_reverse_map_.erase(h.first);
    for (auto &reverse_info : connection_reverse_map_) {
      for_each(reverse_info.second.begin()
            , reverse_info.second.end()
            , [&reverse_info, &h](const HydronId &id) {
            if (h.first == id) {
              reverse_info.second.remove(id);
            }
          });
    }
  }
}

void Colony::SetLearningTheory(const LTType &lt_type) {
  learning_theory_ = CreateLearningTheory(lt_type);
}

void Colony::Ignition() {
  for (auto& h : *hydron_map_) {
    if (h.second.Fire()) {
      idling_hydron_ranking_->remove(h.first);
      idling_hydron_ranking_->push_back(h.first);
    }
  }
}

void Colony::CalculateHeatEffect() {
  for (auto& h : *hydron_map_) {
    h.second.AdaptHeatEffect();
  }
}

void Colony::ApplyFeedback() {
  learning_theory_->Learning(hydron_map_, area_);
  Digest_();
}

int32_t Colony::AddHydron(const Hydron &hydron) {
  // If same ID hydron exist in any colony.
  // Fail AddHydron.
  if (Hydron::SpecifiedHydronExist(hydron.Id())) {
    return -1;
  }
  (*hydron_map_)[hydron.Id()] = hydron;
  (*hydron_map_)[hydron.Id()].RegisterToAllHydronMap();
  idling_hydron_ranking_->push_back(hydron.Id());

  HydronConnections connecting_hydrons = hydron.ConnectingHydrons();
  for (const auto &connection : connecting_hydrons) {
    connection_reverse_map_[connection.first].push_back(hydron.Id());
  }

  return 0;
}

void Colony::DeleteHydron(const HydronId &id) {
  auto it = hydron_map_->find(id);
  if (it == hydron_map_->end()) {
    return;
  }
  HydronConnections connecting_hydrons = it->second.ConnectingHydrons();
  for (const auto &connection : connecting_hydrons) {
    connection_reverse_map_[connection.first].remove(id);
  }

  idling_hydron_ranking_->remove(id);

  hydron_map_->erase(id);
  return;
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

void Colony::SetArea(const Eigen::Vector3f &max, const Eigen::Vector3f &min) {
  area_->max_area_vertix = max;
  area_->min_area_vertix = min;
  area_->volume = common3d::CubeVolumeFromBothEndVertix(
                        area_->max_area_vertix
                        , area_->min_area_vertix);
}

void Colony::Save() const {
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "wb"));

  uint64_t header[2] = {0, 0};
  fwrite(header, sizeof(uint64_t), 2, file.get());
  ExportParameter_(file.get());

  // Export LearningTheory
  LTType lt_type = learning_theory_->LTType();
  fwrite(&lt_type, sizeof(lt_type), 1, file.get());
  learning_theory_->ExportParameter(file.get());

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

  // Import LearningTheory
  LTType lt_type;
  fread(&lt_type, sizeof(lt_type), 1, file.get());
  SetLearningTheory(lt_type);
  learning_theory_->ImportParameter(file.get());

  while (ReadHydron_(file.get()) == 0) {}
}

void Colony::ShowHydronsStatus() const {
  for (auto& h : *hydron_map_) {
    h.second.ShowStatus();
  }
  printf("hydron amount = %" PRIuS "\n", hydron_map_->size());
  learning_theory_->ShowParameter();
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

void Colony::Initialize_() {
  SetArea(Eigen::Vector3f(100.0f, 100.0f, 100.0f)
        , Eigen::Vector3f(-100.0f, -100.0f, -100.0f));
}

void Colony::Digest_() {
  int64_t born_or_death = learning_theory_->BornOrDeath(area_);
  printf("%lu ", hydron_map_->size());
  for (; born_or_death > 0; --born_or_death) {
    Hydron h = learning_theory_->CreateHydron(hydron_map_, area_);
    AddHydron(h);
  }
  for (; born_or_death < 0; ++born_or_death) {
    auto it = idling_hydron_ranking_->begin();
    if (it != idling_hydron_ranking_->end()) {
      DeleteHydron(*it);
    }
  }
}

void Colony::ExportParameter_(FILE *file) const {
  auto ExportFloat = [&file](const float value) -> void {
    fwrite(&value, sizeof(value), 1, file);
  };
  auto ExportVector = [&file, &ExportFloat](const Eigen::Vector3f &v) -> void {
    ExportFloat(v.x());
    ExportFloat(v.y());
    ExportFloat(v.z());
  };
  ExportVector(area_->max_area_vertix);
  ExportVector(area_->min_area_vertix);
}

void Colony::ImportParameter_(FILE *file) {
  struct common3d::MinimumElementVector max;
  struct common3d::MinimumElementVector min;
  fread(&max, sizeof(max), 1, file);
  fread(&min, sizeof(min), 1, file);
  SetArea(Eigen::Vector3f(max.x, max.y, max.z)
      , Eigen::Vector3f(min.x, min.y, min.z));
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
