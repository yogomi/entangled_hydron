// Copyright 2014 Makoto Yano

#ifndef INCLUDE_COLONY_H_
#define INCLUDE_COLONY_H_

#include <list>
#include <string>

#include "./hydron.h"

namespace hydron {

class Colony :public std::list<Hydron> {
 public:
  explicit Colony(const char *colony_name):colony_name_(colony_name) {}
  virtual ~Colony() {}
  void Save() const;
 private:
  Colony() {}
  std::string colony_name_;
};

}  // namespace hydron

#endif  // INCLUDE_COLONY_H_
