// Copyright 2014 Makoto Yano

#include <memory>
#include <list>
#include <algorithm>
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
  std::unique_ptr<FILE, FileDeleter> file(std::fopen("colony.bin", "wb"));

  uint64_t header = 0;
  fwrite(&header, sizeof(header), 1, file.get());
  std::for_each(this->begin(), this->end()
      , [&](Hydron h) {h.ExportStatus(file.get());});
}

}  // namespace hydron
