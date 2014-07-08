// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <cstdint>
#include <list>

class HydronId {
 public:
  HydronId(): x_(0), y_(0), z_(0) {}
  HydronId(int32_t x, int32_t y, int32_t z)
    : x_(x), y_(y), z_(z) {}
  ~HydronId() {}
 private:
  int32_t x_, y_, z_;
};

class Hydron {
 public:
  Hydron();
  ~Hydron() {}
 private:
  std::list<HydronId> connecting_hydron_;
  HydronId id_;
  uint32_t threshold_;
  uint32_t strength_;
};

#endif  // INCLUDE_HYDRON_H_
