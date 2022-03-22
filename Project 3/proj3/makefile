# Copyright 2022  CSCE311
cc = g++  # use g++ compiler

flags = -std=c++17  # compile with C++ 17 standard
flags += -Wall  # compile with all warnings
flags += -g  # produce debugging information
#flags += -I .. # add pwd directory to src include path
#flags += -pthread # support for POSIX threads

link = $(cc) $(flags) -o
compile = $(cc) $(flags) -c


# socket-cli-srv
#
#socket-cli-srv: socket_cli_srv.cc
#	$(link) $@ $^
make: text-server text-client


# Clean up
clean:
	$(RM) text-client text-server

text-client: project2client.cpp
	$(link) $@ $^

text-server: project2server.cpp
	$(link) $@ $^
