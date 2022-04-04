
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<unistd.h>
#include<sys/sysinfo.h>
#include <stdio.h>

#include<cassert> // assert
#include<cerrno> // errno
#include<cstddef> // size_t
#include<cstdlib> // exit
#include<cstring> //strncpy

#include<string>
#include<iostream>
#include<iostream>
#include<sstream>
#include<fstream>

using std::string;
using std::cout;
using std::endl;


// client side
// arg 1 = domain socket name
// arg 2 = file to search through
// arg 3 = what to search
int main (int argc, char *argv[]) {
  if (argc != 4) {
    return 1;
  }

  // on connection, write
  return 0;
}
