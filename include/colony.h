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

  // Save all Hydrons to file.
  // Filename is colony_name_ + .bin.
  void Save() const;

  // Clear all Hydrons and read file and set new Hydrons.
  // Filename is colony_name_ + .bin.
  // If could not open file, remain empty Hydron list.
  void Load();

  void Print() const;
 private:
  Colony() {}
  int64_t ReadHydron_(FILE *file);
  std::string FileName_() const;
  std::string colony_name_;
};

}  // namespace hydron

#endif  // INCLUDE_COLONY_H_
