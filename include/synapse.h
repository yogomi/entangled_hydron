// Copyright 2014 Makoto Yano

#ifndef INCLUDE_SYNAPSE_H_
#define INCLUDE_SYNAPSE_H_

#include <cstdint>

class SynapseId {
 public:
  SynapseId():id_[0](0), id_[1](0), id_[2](0) {}
  ~SynapseId() {}
 private:
  uint32_t id_[3];
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
