// write all the stuff in here
// look at CSCE 416 proj 1 for implementation of the server/client
// lewisjs4 github in ipc
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

#include<cassert> // assert
#include<cerrno> // errno
#include<cstddef> // size_t
#include<cstdlib> // exit
#include<cstring> //strncpy

#include<string>
#include<iostream>
using std::string;
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
        sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd < 0) { // error check
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }

        unlink(sock_path.c_str()); // delete file if it exists
                                      // UNIX style tho, non-portable
        // bind socket to server
        int success = bind(sock_fd,
                          reinterpret_cast<const sockaddr*>(&sock_add_),
                          sizeof(sock_add_));
      if (success < 0) { // error check
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      // listen for client
      size_t kMax_client_conns = 5; // may need to change this to "n-1"
      success = listen(sock_fd, kMax_client_conns);

      if (success < 0) { // error check
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      const size_t kRead_buffer_size = 32; // again, change this
      char read_buffer[kRead_buffer_size];
      int bytes_read;

      // look for connector
      while (true) {
        client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
        if (client_req_sock_fd < 0) { // error check
          std::cerr << strerror(errno) << std::endl;
          continue;
        }
      }

      std::cout << "Client connected \n";

      // read data from the client
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      const char kKill_msg[] = "quit";

      // loop until empty input
      while (bytes_read > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::cout << "Server shutted down... bwomp bwomp" << std::endl;

          bytes_read = 0;
          break;
        }

        std::cout << "read" << bytes_read << "bytes: ";
        std::cout.write(read_buffer, bytes_read) << std::endl;

        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      }

      if (bytes_read == 0) {
        std::cout << "Client DC-ed" << std::endl;

        close(client_req_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
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
