// Copyright 2022 CSCE 311
//

#include <proj3/inc/producer.h>

#include <cstddef>
#include <iostream>


// act as the "client"
// name name
int main(int argc, char* argv[]) {
  assert(argc == 3 && "./text-client dat/<text_file> \"<string_search>\"");
  char name1[] = "stringbean";
  char name2[] = "broccoliandpeas";
  logger::Producer log_writer(name1, name2);

  std::cout << "Sending: > ";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::cin.getline(buffer, kBuffer_size);
  buffer[std::cin.gcount() + 1] = '\n';
  std::string msg(buffer);
  std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
  while (std::cin.gcount() == kBuffer_size - 1) {
    std::cin.clear();
    std::cin.getline(buffer, kBuffer_size);
    std::cout << "Bytes read: " << std::cin.gcount() << std::endl;
    buffer[std::cin.gcount() + 1] = '\0';
    msg += buffer;
  }
  std::cout << "Sent: " << msg << std::endl;
  log_writer.Produce(std::string(msg));

  return 0;
}
