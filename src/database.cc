// Copyright 2014 Makoto Yano

#include <mysql++.h>
#include <stdlib.h>

#include "include/database.h"

Database::Database(void) {
  Init();
}

void Database::Init(void) {
  try {
    mysqlpp::Connection conn(false);
    conn.connect("synapse", "localhost", "synapse", "password");
    conn.query();
  } catch(mysqlpp::BadQuery error) {
    std::cout << "Error: " << error.what() << std::endl;
  }
  return;
}
