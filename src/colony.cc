// Copyright 2014 Makoto Yano

#include <memory>
#include <string>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "./hydron.h"
#include "./colony.h"

namespace hydron {

struct FileDeleter {
  void operator()(FILE *ptr) const {
    std::fclose(ptr);
  }
};

void Colony::AddHydron(const Hydron &hydron) {
  hydron_map_[hydron.Id()] = hydron;
}

void Colony::ConnectHydronToHydron(const Hydron &from
                                , const Hydron &to
                                , const float weight) {
  ConnectHydronToHydron(from.Id(), to.Id(), weight);
}

void Colony::ConnectHydronToHydron(const HydronId &from
                                , const HydronId &to
                                , const float weight) {
  if (hydron_map_.find(from) != hydron_map_.end()) {
    hydron_map_[from].ConnectTo(to, weight);
    connection_reverse_map_[to].push_back(from);
  }
}

void Colony::Save() const {
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "wb"));

  uint64_t header[2] = {0, 0};
  fwrite(header, sizeof(header), 1, file.get());
  for (const auto& h : hydron_map_) {
    h.second.ExportStatus(file.get());
  }
}

void Colony::Load() {
  hydron_map_.clear();
  connection_reverse_map_.clear();
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "rb"));
  if (file.get() == nullptr) {
    return;
  }
  uint64_t header[2];
  fread(header, sizeof(header), 1, file.get());

  while (ReadHydron_(file.get()) == 0) {}
}

void Colony::Print() const {
  for (auto& h : hydron_map_) {
    h.second.ShowStatus();
  }
}

std::string Colony::FileName_() const {
  return colony_name_ + ".bin";
}

int64_t Colony::ReadHydron_(FILE *file) {
  struct common3d::MinimumElementVector v;
  struct HydronParameter parameter;
  uint64_t connecting_hydron_count;

  if (fread(&v, sizeof(v), 1, file) <= 0) {
    return -1;
  }
  Hydron h(v.x, v.y, v.z);
  fread(&v, sizeof(v), 1, file);
  h.SetHeadDirection(v.x, v.y, v.z);

  fread(&parameter, sizeof(parameter), 1, file);
  h.SetParameter(parameter);

  fread(&connecting_hydron_count, sizeof(connecting_hydron_count), 1, file);
  hydron_map_[h.Id()] = h;

  for (uint64_t i = 0; i < connecting_hydron_count; ++i) {
    float weight;
    fread(&v, sizeof(v), 1, file);
    fread(&weight, sizeof(weight), 1, file);
    ConnectHydronToHydron(h.Id(), HydronId(v.x, v.y, v.z), weight);
  }

  return 0;
}

}  // namespace hydron
