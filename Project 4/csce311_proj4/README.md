# CSCE311 Project 03 - Shared Memory - Robert Sturman

Included files :
  makefile
    makes the project in c++
    clean
      -remove the executable files

  writer.cc (./text-server)
    creates a "server" for shared memory
      -Creates a shared memory location
      -Accepts client's file path and attempts to transfer that file to another shared memory location
      -server does not quit, and stays active


  generator.cc (./text-client <FILE NAME> <SEARCH ITEM>)
    Acts as a "client" for this "server" of shared memory
      -Creates a shared memory location
      -Writes the name of the file to the shared memory
      -Once the server has read the filename, reads the supplied file from another shared memory location and parses it
      -prints all matching lines from the shared memory and the search item 
