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
  logger::Producer log_writer(name1, name2); //create shared memory location

  //std::cout << "Sending: > ";

  const size_t kBuffer_size = 64;
  char buffer[kBuffer_size];
  std::string msg(buffer);
  std:: ifstream fileInput;

  msg = argv[1];
  log_writer.Produce(std::string(msg)); //send file path to server
  unsigned int currentline = 0;
  int numberline = 1;
  std::string line;
  std::string big;
  fileInput.open("temp.txt");
  // NOTE: THREADLESS IMPLEMENTATION
  // search though shared memory location for a match
  if (fileInput.is_open()) {
    while(getline(fileInput, line)) {
      currentline++;
      if (line.find(argv[2], 0) != std::string::npos) {
        big += std::to_string(numberline) + "\t" + line + "\n";
        numberline++;
      }
    }
  }
  std::cout<< big; // print all matches


  remove("temp.txt"); // Destroy shared memory location
  return 0;
}
