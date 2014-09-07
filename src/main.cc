// Copyright 2014 Makoto Yano

#include <memory>
#include <map>
#include <algorithm>

#include "./vector.h"
#include "./hydron.h"
#include "./colony.h"
#include "./database.h"
#include "learning_theory/learning_theory.h"

using hydron::Colony;
using hydron::AlliedColonies;
using hydron::Hydron;

Colony CreateColony2();
std::shared_ptr<Colony> CreateColony();

int main() {
  std::shared_ptr<Colony> colony(new Colony("test"));
  AlliedColonies brain;
  // colony.Save();
  printf("--------------------------------------");
  printf("--------------------------------------");
  printf("--------------------------------------");
  printf("--------------------------------------\n");
  colony->SetLearningTheory(hydron::FEED_LEARNING);
  brain.SetColony(colony);
  colony->Load();

      return 0;
  int i = 0;
  while (true) {
    brain.Beat();
    if (i > 5000) {
      colony->ShowHydronsStatus();
      colony->Save();
      i = 0;
    }
    i++;
  }

  return 0;
}

Colony CreateColony2() {
  Colony colony("test_colony888");
  colony.AddHydron(Hydron(2, 4, 3));
  colony.ConnectHydronToHydron(hydron::HydronId(3, 4, 3)
                              , hydron::HydronId(-12, 3, -4));
  colony.AddHydron(Hydron());
  colony.ConnectHydronToHydron(hydron::HydronId(2, 4, 3)
                              , hydron::HydronId(3, 4, 5));
  colony.ConnectHydronToHydron(hydron::HydronId(0, 0, 0)
                              , hydron::HydronId(3, 4, 5));
  colony.AddHydron(Hydron(5, 6, 2));
  colony.ConnectHydronToHydron(hydron::HydronId(2, 4, 3)
                              , hydron::HydronId(22003, 4313, 334135));
  colony.ConnectHydronToHydron(hydron::HydronId(5, 6, 2)
                              , hydron::HydronId(22003, 4313, 334135));
  colony.ConnectHydronToHydron(hydron::HydronId(0, 0, 0)
                              , hydron::HydronId(22003, 4313, 334135));
  return colony;
}

std::shared_ptr<Colony> CreateColony() {
  std::shared_ptr<Colony> colony(new Colony("test_colony"));
  colony->AddHydron(Hydron(2, 9, 3));
  colony->ConnectHydronToHydron(hydron::HydronId(2, 9, 3)
                              , hydron::HydronId(5, 6, 2));
  colony->AddHydron(Hydron(1, 3, 4));
  colony->ConnectHydronToHydron(hydron::HydronId(2, 9, 3)
                              , hydron::HydronId(1, 3, 4));
  colony->ConnectHydronToHydron(hydron::HydronId(1, 3, 4)
                              , hydron::HydronId(5, 6, 2));
  colony->AddHydron(Hydron(5, 6, 2));
  colony->ConnectHydronToHydron(hydron::HydronId(2, 9, 3)
                              , hydron::HydronId(5, 6, 2));
  colony->ConnectHydronToHydron(hydron::HydronId(5, 6, 2)
                              , hydron::HydronId(5, 6, 2));
  colony->ConnectHydronToHydron(hydron::HydronId(1, 3, 4)
                              , hydron::HydronId(2, 9, 3));
  return colony;
}

