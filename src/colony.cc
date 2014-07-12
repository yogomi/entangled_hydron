// Copyright 2014 Makoto Yano

#include <memory>
#include <list>
#include <algorithm>

#include "./hydron.h"
#include "./colony.h"

namespace hydron {

struct FileDeleter {
  void operator ()(FILE *ptr) const {
    std::fclose(ptr);
  }
};

void Colony::Save() const {
  std::for_each(this->begin(), this->end()
      , [](Hydron h) {h.ShowStatus();});
  std::unique_ptr<FILE, FileDeleter> file(std::fopen("colony.bin", "wb"));
}

}  // namespace hydron
