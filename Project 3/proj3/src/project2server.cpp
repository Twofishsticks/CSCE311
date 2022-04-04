// write all the stuff in here

#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/sysinfo.h>
#include <stdio.h>

#include<algorithm>
#include<cassert> // assert
#include<cerrno> // errno
#include<cstddef> // size_t
#include<cstdlib> // exit
#include<cstring> //strncpy
#include<filesystem> // for looking for file
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>

using std::string;
using std::endl;
using std::cout;
using std::filesystem::recursive_directory_iterator;

    bool findFile(string fileName) { // works
      string path = "./dat";
      string fullFile = "./dat/" + (fileName) ;
      //cout << fullFile << endl << endl;
      for (const auto & file : recursive_directory_iterator(path)) {
        //cout << file.path() << " vs " << fullFile << endl;
        if (file.path().compare(fullFile) == 0) {
          return true;
        }
      }
      return false;
    }


    void findEach(string fileName, string searchItem, int socket_fd, char* write_buffer) {
      int bytes_sent = 0;
      string line;
      string fullFile = "./dat/" + fileName;
      std::ifstream gigaFile(fullFile);
      while (getline(gigaFile,line)) {
        //cout << line << endl;
        if (strstr(line.c_str(), searchItem.c_str())) {
          line+="PEEPEEPOOPOO"; // change this
          bytes_sent += send(socket_fd, line.data(), line.size(), 0);
          //bytes_sent += write(socket_fd, write_buffer, line.length()-1);
          //cout << line <<endl;
        }
      }
      cout << "bytes sent: " << bytes_sent;
    }
};

// handle everything in here
// arg 1 = domain socket name
int main (int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  
  return 0;
}
