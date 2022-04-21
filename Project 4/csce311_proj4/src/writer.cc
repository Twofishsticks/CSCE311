/// Copyright 2022 CSCE 311
//

#include <csce311_proj4/inc/consumer.h>

#include <cstddef>
#include <iostream>
#include <filesystem>
#include <fstream>

using std::cout;
using std::endl;
// Deletes kLogger memory (calls destructor)
void LoggerSigTermHandler(int sig);


logger::Consumer* log_writer;


int main(/*int argc, char* argv[]*/) {
  //assert(argc == 2 && "usage: consumer <log_file_name>");

  // set SIGTERM signal handler to unlink shm
  ::signal(SIGTERM, LoggerSigTermHandler);
  ::signal(SIGINT, LoggerSigTermHandler);

  // build Consumer and start writing to file
  log_writer = new logger::Consumer();
  cout << "SERVER STARTED" << endl;
  // see consumer.cc for documented steps
  log_writer->Consume("dante.txt");

  return 0;
}


void LoggerSigTermHandler(int sig) {
  if (sig == SIGTERM || sig == SIGINT) {
    delete log_writer;
  }
}
