// Copyright 2014 Makoto Yano

#include <list>
#include <algorithm>

#include "include/database.h"
#include "include/hydron.h"

namespace hydron {

void Save(const Colony &colony) {
  std::for_each(colony.begin(), colony.end()
      , [](Hydron h) {h.ShowStatus();});
}

}  // namespace hydron
