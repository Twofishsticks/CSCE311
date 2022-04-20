// Copyright 2022 CSCE 311
//

#include <csce311_proj4/inc/producer.h>

#include <cstddef>
#include <iostream>

int main(int argc, char* argv[]) {
  logger::Producer log_writer;

  //std::cout << "Sending: > ";

  //const size_t kBuffer_size = 32;
  //char buffer[kBuffer_size];

  //std::cin.getline(buffer, kBuffer_size);
  std::string msg(argv[1]);
  // should send the filename first
  /*
  while (std::cin.fail()) {  // did not get to \n in getline
    std::cin.clear();  // reset fail bit

    std::cin.getline(buffer, kBuffer_size);

    msg += std::string(buffer);
  }
  */
  std::cout << "Sent: " << argv[1] << std::endl;
  //see producer.cc for documented steps
  // sent what needs to be written to the file chosen by writer.cc
  log_writer.Produce(argv[1]);

  //need to wait, then read the file

  return 0;
}
