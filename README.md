# CSCE311 Project 02 - Text Client/Server - Robert Sturman

Included files :
  makefile
    makes the project in c++

    text-server
      -makes the file text-server project2server.cpp
    text-client
      -makes the file text-client from project2client.cpp
    clean
      -remove the executable files
  project2server.cpp (./text-server <DOMAIN NAME>)
    Opens a socket for clients to connect
      -After connection, recieves two strings
        -file name, search item
      -Checks if the file is avaliable in the ./dat directory
        -If not, sends "INVALID FILE", disconnects client
      -Compares the search item to each line in the chosen file
        -Upon match, sends the line with the match to the client
        -Once file is completed, prints the total number of bytes sent


  project2client.cpp (./text-client <DOMAIN NAME> <FILE NAME> <SEARCH ITEM>)
    Connects to socket opened by project2server.cpp
      -After connection, sends two strings
        -file name, search item
      -Recieves lines containing found item from the server and prints them
