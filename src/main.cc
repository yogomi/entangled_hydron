// Copyright 2014 Makoto Yano

#include <list>
#include <algorithm>

#include "./hydron.h"
#include "./colony.h"
#include "./database.h"

using hydron::Colony;
using hydron::Hydron;

Colony CreateColony() {
  Colony colony("test_colony");
  colony.push_back(Hydron(2, 4, 3));
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {
    h.ConnectTo(2, 4, 3);
  });
  colony.push_back(Hydron());
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {
    h.ConnectTo(3, 4, 5);
  });
  colony.push_back(Hydron(5, 6, 2));
  std::for_each(colony.begin(), colony.end()
      , [](Hydron &h) {
    h.ConnectTo(22003, 44213, 334135);
  });
  return colony;
}

int main() {
  Colony colony = CreateColony();
  colony.Save();
  colony.Print();
  return 0;
}
