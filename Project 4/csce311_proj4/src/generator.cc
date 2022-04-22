// Copyright 2022 CSCE 311
//

#include <csce311_proj4/inc/producer.h>

#include <cstddef>
#include <iostream>

int main(int argc, char* argv[]) {
  logger::Producer log_writer;

  std::string msg(argv[1]);

  std::cout << "Sent: " << argv[1] << std::endl;

  log_writer.Produce(argv[1]);

  return 0;
}
