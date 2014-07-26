// Copyright 2014 Makoto Yano

#include <memory>
#include <map>
#include <algorithm>

#include "./vector.h"
#include "./hydron.h"
#include "./colony.h"
#include "./database.h"

using hydron::Colony;
using hydron::Hydron;

std::shared_ptr<Colony> CreateColony() {
  std::shared_ptr<Colony> colony(new Colony("test_colony"));
  colony->AddHydron(Hydron(2, 4, 3));
  colony->ConnectHydronToHydron(hydron::HydronId(2, 4, 3)
                              , hydron::HydronId(-12, 3, -4));
  colony->AddHydron(Hydron());
  colony->ConnectHydronToHydron(hydron::HydronId(2, 4, 3)
                              , hydron::HydronId(3, 4, 5));
  colony->ConnectHydronToHydron(hydron::HydronId(0, 0, 0)
                              , hydron::HydronId(3, 4, 5));
  colony->AddHydron(Hydron(5, 6, 2));
  colony->ConnectHydronToHydron(hydron::HydronId(2, 4, 3)
                              , hydron::HydronId(22003, 4313, 334135));
  colony->ConnectHydronToHydron(hydron::HydronId(5, 6, 2)
                              , hydron::HydronId(22003, 4313, 334135));
  colony->ConnectHydronToHydron(hydron::HydronId(0, 0, 0)
                              , hydron::HydronId(22003, 4313, 334135));
  return colony;
}

int main() {
  std::shared_ptr<Colony> colony = CreateColony();
  colony->Print();
  colony->Save();
  colony->Load();
  printf("--------------------------------------");
  printf("--------------------------------------");
  printf("--------------------------------------");
  printf("--------------------------------------\n");
  colony->Print();
  std::map<common3d::Vector, Hydron> brain;
  brain[common3d::Vector(3.0f, 3.0f, 3.0f)];
  return 0;
}
