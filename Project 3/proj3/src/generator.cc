// Copyright 2022 CSCE 311
//

#include <proj3/inc/producer.h>

#include <cstddef>
#include <iostream>
#include <fstream>

// act as the "client"
int main(int argc, char* argv[]) {
  assert(argc == 3 && "./text-client dat/<text_file> \"<string_search>\"");
  char name1[] = "stringbean";
  char name2[] = "broccoliandpeas";
  logger::Producer log_writer(name1, name2);

  //std::cout << "Sending: > ";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::string msg(buffer);
  std:: ifstream fileInput;
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
  // when match is found do # tab string newline as cout (from string)
  unsigned int currentline = 0;
  int numberline = 1;
  std::string line;
  std::string big;
  fileInput.open("temp.txt");
  if (fileInput.is_open()) {
    while(getline(fileInput, line)) {
      currentline++;
      if (line.find(argv[2], 0) != std::string::npos) {
        big += std::to_string(numberline) + "\t" + line + "\n";
        numberline++;
      }
    }
  }
  std::cout<< big;


  //msg = argv[2];
  //log_writer.Produce(std::string(msg)); // these work lol



  return 0;
}
