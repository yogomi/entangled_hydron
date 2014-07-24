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

void Colony::Save() const {
  std::unique_ptr<FILE, FileDeleter> file(
      std::fopen(FileName_().data() , "wb"));

  uint64_t header[2] = {0, 0};
  fwrite(header, sizeof(header), 1, file.get());
  std::for_each(this->begin(), this->end()
      , [&file](Hydron h) {h.ExportStatus(file.get());});
}

void Colony::Load() {
  this->clear();
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
  std::for_each(this->begin(), this->end()
      , [](Hydron h) {h.ShowStatus();});
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
  for (uint64_t i = 0; i < connecting_hydron_count; ++i) {
    float weight;
    fread(&v, sizeof(v), 1, file);
    fread(&weight, sizeof(weight), 1, file);
    h.ConnectTo(v.x, v.y, v.z, weight);
  }

  this->push_back(h);

  return 0;
}

}  // namespace hydron
