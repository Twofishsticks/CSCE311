//all the client stuff goes here
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

class DomainSocketSocket : public UnixDomainSocket {
  public :
    using ::UnixDomainSocket::UnixDomainSocket;

    void RunClient() {
      // open UNIX socket
      int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (socket_fd < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      // connect to socket
      int success = connect(socket_fd,
                            reinterpret_cast<const sockaddr*>(&sock_add_),
                            sizeof(sock_add_));
      if (success < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      // write to socket
      ssize_t kWrite_buffer_size = 64;
      char write_buffer[kWrite_buffer_size];
      //send it
      while(true) {
        std::cin.getline(write_buffer, kWrite_buffer_size);
        while (std::cin.gcount() > 0) {
          if (std::cin.gcount() == kWrite_buffer_size - 1 && std::cin.fail()) {
            std::cin.clear();
          }
          ssize_t bytes_wrote = write(socket_fd, write_buffer, std::cin.gcount());
          std::cout << "sent" << std::cin.gcount() << " bytes" << std::endl;
          if (bytes_wrote < 0) {
            std::cerr << strerror(errno) << std::endl;
            exit(-1);
          }

          if (bytes_wrote == 0) {
            std::clog << "Server dropped connection! L" << std:: endl;
            exit(-2);
          }

          std::cin.getline(write_buffer, kWrite_buffer_size);

        }
      }
    }
};
