// Copyright 2014 Makoto Yano

#ifndef INCLUDE_DATABASE_H_
#define INCLUDE_DATABASE_H_

class Database {
 public:
  Database(void);
  ~Database(void) {}
 private:
  void Init(void);
  char* database_name_;
};

#endif  // INCLUDE_DATABASE_H_
