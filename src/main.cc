// Copyright 2014 Makoto Yano

#include <list>
#include <map>
#include <algorithm>

#include "./vector.h"
#include "./hydron.h"
#include "./colony.h"
#include "./database.h"

using hydron::Colony;
using hydron::Hydron;

Colony CreateColony() {
  Colony colony("test_colony");
  colony.push_back(Hydron(2, 4, 3));
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {  // NOLINT
    h.ConnectTo(2, 4, 3);
  });
  colony.push_back(Hydron());
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {  // NOLINT
    h.ConnectTo(3, 4, 5);
  });
  colony.push_back(Hydron(5, 6, 2));
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {  // NOLINT
    h.ConnectTo(22003, 44213, 334135);
  });
  return colony;
}

int main() {
  Colony colony = CreateColony();
  colony.Print();
  colony.Save();
  colony.Load();
  colony.Print();
  std::map<common3d::Vector, Hydron> brain;
  brain[common3d::Vector(3.0f, 3.0f, 3.0f)];
  return 0;
}
