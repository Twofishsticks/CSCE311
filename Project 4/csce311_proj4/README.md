# CSCE311 Project 04 - Mem Mapping - Robert Sturman

Included files :
  makefile
    make
      -makes the project in c++, creates executable files
    clean
      -remove the executable files and the .o and .t files

  writer.cc (./text-server)
    creates a "server" for shared memory

      -Creates a shared memory location
      -Accepts client's file path and attempts to transfer that file to another shared memory location
      -server does not quit, and stays active


  generator.cc (./text-client <FILE NAME>)
    Acts as a "client" for this "server" of shared memory

      -Creates a shared memory location
      -Writes the name of the file to the shared memory
      -Once the server has read the filename, reads the supplied file from another shared memory location and parses it
      -prints all matching lines from the shared memory and the search item

  file_backed_mmap.c
    Handles mmapping of communication between server and client

      -Returns into a char array, which is "shared" between server and client
