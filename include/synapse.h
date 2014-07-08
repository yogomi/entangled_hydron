// Copyright 2014 Makoto Yano

#ifndef INCLUDE_SYNAPSE_H_
#define INCLUDE_SYNAPSE_H_

#include <cstdint>

class SynapseId {
 public:
  SynapseId(): x_(0), y_(0), z_(0) {}
  SynapseId(int32_t x, int32_t y, int32_t z)
    : x_(x), y_(y), z_(z) {}
  ~SynapseId() {}
 private:
  int32_t x_, y_, z_;
};

class Synapse {
 public:
  Synapse();
  ~Synapse() {}
 private:
  SynapseId id_;
  uint32_t threshold_;
  uint32_t strength_;
};

#endif  // INCLUDE_SYNAPSE_H_
