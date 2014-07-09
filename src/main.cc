// Copyright 2014 Makoto Yano

#include <list>

#include "include/hydron.h"
#include "include/database.h"

using hydron::Colony;
using hydron::Hydron;

Colony CreateColony() {
  Colony colony;
  colony.push_back(Hydron(2, 4, 3));
  colony.push_back(Hydron());
  colony.push_back(Hydron(5, 6, 2));
  return colony;
}

int main() {
  Colony colony = CreateColony();
  return 0;
}
