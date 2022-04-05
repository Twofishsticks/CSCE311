// Copyright 2022 CSCE 311
//

#include <proj3/inc/producer.h>

#include <cstddef>
#include <iostream>


// act as the "client"
int main(int argc, char* argv[]) {
  assert(argc == 3 && "./text-client dat/<text_file> \"<string_search>\"");
  char name1[] = "stringbean11";
  char name2[] = "broccoliandpeas11";
  logger::Producer log_writer(name1, name2);

  //std::cout << "Sending: > ";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::string msg(buffer);
  /* send one line, only reads the string
  std::cin.getline(buffer, kBuffer_size);
  buffer[std::cin.gcount() + 1] = '\n';
  std::string msg(buffer);
  std::cout << "Bytes read: " << std::cin.gcount() << std::endl;

  // read a LOT of stuff, determined by buffer size
  while (std::cin.gcount() == kBuffer_size - 1) {
    std::cin.clear();
    std::cin.getline(buffer, kBuffer_size);
    std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
    buffer[std::cin.gcount() + 1] = '\0';
    msg += buffer;
  }
  std::cout << "Sent: " << msg << std::endl;
  // end reading sending string
  */
  //std::cout<<msg<<std::endl;
  msg = argv[1];
  log_writer.Produce(std::string(msg));
  //msg = argv[2];
  //log_writer.Produce(std::string(msg)); // these work lol



  return 0;
}
