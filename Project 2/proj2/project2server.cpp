// write all the stuff in here
// look at CSCE 416 proj 1 for implementation of the server/client
// lewisjs4 github in ipc
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<sys/sysinfo.h>

#include<cassert> // assert
#include<cerrno> // errno
#include<cstddef> // size_t
#include<cstdlib> // exit
#include<cstring> //strncpy
#include<filesystem> // for looking for file
#include<string>
#include<iostream>
#include<fstream>

using std::string;
using std::endl;
using std::cout;
using std::filesystem::recursive_directory_iterator;
class UnixDomainSocket {

  public:
    explicit UnixDomainSocket(const char *sock_path) {
      string sock_path_ = string(sock_path);
      sock_add_ = {}; // cool struct thing
      sock_add_.sun_family = AF_UNIX; // set to UNIX socket type

      strncpy(sock_add_.sun_path + 1,
              sock_path,
              sizeof(sock_add_.sun_path)-2); // -2 checks for starting or
                                             // ending zeros
    }

  protected:
    sockaddr_un sock_add_; //sock add from sys/un.h
    string sock_path;           // string is *char
};

class DomainSocketServer : public UnixDomainSocket {
  public:
    using ::UnixDomainSocket::UnixDomainSocket;

      void RunServer() const {
        int sock_fd;  // file descriptor
        int client_req_sock_fd; // client connect request to sock_fd
        // making the socket
        //cout << "server run" << endl;
        sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd < 0) { // error check
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }
        //cout << "sock_fd" << endl;
        unlink(sock_path.c_str()); // delete file if it exists
                                      // UNIX style tho, non-portable
        // bind socket to server
        //cout << "unlink" << endl;
        int success = bind(sock_fd,
                          reinterpret_cast<const sockaddr*>(&sock_add_),
                          sizeof(sock_add_));
      if (success < 0) { // error check
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
      //cout << "binding" << endl;
      // listen for client
      size_t kMax_client_conns = get_nprocs_conf();
      //cout << "starting listen" << endl;
      success = listen(sock_fd, kMax_client_conns);

      if (success < 0) { // error check
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      const size_t kRead_buffer_size = 32; // again, change this
      char read_buffer[kRead_buffer_size];
      int bytes_read;

      // look for connector
      //cout << "beginnning while loop" << endl;
      while (true) {
        client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
        //cout << "accept" << endl;
        if (client_req_sock_fd < 0) { // error check
          std::cerr << strerror(errno) << std::endl;
          continue;
        } else {
          cout << "SERVER STARTED" << endl;
          cout << "\t MAX CLIENTS: " << kMax_client_conns;
        }


      std::cout << "Client connected \n";

      // read data from the client
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      const char kKill_msg[] = "quit";

      // All of this is for sending/recieving messages
      // loop until empty input
      while (bytes_read > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::cout << "Client has disconnected" << std::endl;
          bytes_read = 0;
          return;
        }

        std::cout << "read " << bytes_read << " bytes: ";
        std::cout.write(read_buffer, bytes_read) << std::endl;

        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      }
      //
      //string finderFile =

      if (bytes_read == 0) {
        std::cout << "Client DC-ed" << std::endl;

        close(client_req_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
    }
    };

    bool findFile(string fileName) {
      string path = "./dat";
      for (const auto & file : recursive_directory_iterator(path)) {
        if (file.path().compare(fileName) == 0) {
          return true;
        }
      }
      return false;
    }
    bool findEach(string fileName, string searchItem) {
      string line;
      string fullFile = "./dat" + fileName;
      std::ifstream gigaFile(fullFile);
      while (getline(gigaFile,line)) {
        if (line.find(searchItem)!= string::npos) {
          cout << line << endl;
        }
      }
      return false;
    }
};

// handle everything in here
// arg 1 = domain socket name
int main (int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  DomainSocketServer server = DomainSocketServer(argv[1]);
  server.RunServer();
}
