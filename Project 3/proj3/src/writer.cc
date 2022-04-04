// Copyright 2022 CSCE 311
//

#include <proj3/inc/consumer.h>

#include <cstddef>
#include <iostream>


// Deletes kLogger memory (calls destructor)
void LoggerSigTermHandler(int sig);


logger::Consumer* log_writer;

// act as the "server"
// name, name, file to write to
int main(int argc, char* argv[]) {
  assert(argc == 1 && "usage: ./text-server");


  // set SIGTERM signal handler to unlink shm
  ::signal(SIGTERM, LoggerSigTermHandler);
  ::signal(SIGINT, LoggerSigTermHandler);

  // build Consumer and start writing to file
  char name1[] = "stringbean111";
  char name2[] = "broccoliandpeas111";
  log_writer = new logger::Consumer(name1, name2);

  // Everything dealing with the client themselves is located in Consume
  log_writer->Consume("temp.txt");

  return 0;
}


void LoggerSigTermHandler(int sig) {
  if (sig == SIGTERM || sig == SIGINT)
    delete log_writer;
}
