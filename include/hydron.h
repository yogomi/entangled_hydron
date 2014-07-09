// Copyright 2014 Makoto Yano

#ifndef INCLUDE_HYDRON_H_
#define INCLUDE_HYDRON_H_

#include <cstdint>
#include <list>

namespace hydron {

class HydronId {
 public:
  HydronId(): x_(0), y_(0), z_(0) {}
  HydronId(const int32_t x, const int32_t y, const int32_t z)
    : x_(x), y_(y), z_(z) {}
  ~HydronId() {}

  int32_t x() const {
    return x_;
  }
  int32_t y() const {
    return y_;
  }
  int32_t z() const {
    return z_;
  }
 private:
  int32_t x_, y_, z_;
};

class Hydron {
 public:
  Hydron();
  Hydron(const int32_t x, const int32_t y, const int32_t z);
  void ChangeId(const int32_t x, const int32_t y, const int32_t z);
  ~Hydron() {}

  void ConnectTo(const int32_t x, const int32_t y, const int32_t z);
  void ConnectTo(const HydronId &id);
  void ConnectTo(const Hydron& h);

  HydronId Id() const {
    return id_;
  }
  void ShowStatus();
 private:
  std::list<HydronId> connecting_hydron_;
  HydronId id_;
  uint32_t threshold_;
  uint32_t strength_;
  uint32_t step_;
  uint32_t refractory_period_;
};

typedef std::list<Hydron> Colony;

}  // namespace hydron

#endif  // INCLUDE_HYDRON_H_
