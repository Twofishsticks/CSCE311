// Copyright 2022 CSCE 311
//

#include <csce311_proj4/inc/producer.h>
#include <iterator>
namespace logger {

Producer::Producer() : log_sig_(log_sig_name_) {
  // get shared memory log signal named semaphore
  //   created by and locked by consumer
  log_sig_.Open();
}

namespace {

void HandleError(const char msg[]) {
  std::cerr << "Source: " << msg << std::endl;
  std::cerr << '\t' << ::strerror(errno) << std::endl;
  ::exit(errno);
}


std::tuple<int, off_t> OpenFile(const char file_name[], int flags) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);
  if (fd < 0) {
    std::cerr << ::strerror(errno) << std::endl;
    ::exit(errno);
  }

  // get size of file
  struct ::stat f_stats;
  if (::fstat(fd, &f_stats) < 0)
    HandleError("Open File");

  return {fd, f_stats.st_size};
}

}  // anonymous namespace

//GENERATOR
int Producer::Produce(const std::string& msg) {
  std::cout<<msg<<"message"<<std::endl;
  // open buffer file and get stats
  int buf_fd;
  long int buf_size;
  std::tie(buf_fd, buf_size) = OpenFile(buf_file_name_, O_RDWR);

  // assure file empty and get map to page cache
  if (buf_size > 0) {
    std::cerr << "Transfer file is not empty" << std::endl;
    ::exit(1);
  }

  // make file big enough
  if (static_cast<unsigned long int>(buf_size) < msg.length() + 1)
    if (::fallocate(buf_fd, 0, 0, msg.length() + 1) < 0)  // need to add \n
      HandleError("fallocate");
  char *buf_file_addr = static_cast<char *>(::mmap(nullptr,
                                                   msg.length() + 1,
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED,
                                                   buf_fd,
                                                   0));
  std::cout << "buf_file_addr at line 57: "<<buf_file_addr<< std::endl;
  // should be nothing: this only links the mmap
  if (buf_file_addr == MAP_FAILED)
    HandleError("Buffer file map");
  if (::close(buf_fd) < 0)
    HandleError("Buffer file close");

  // copy string msg into memory mapped file in page cache
  for (size_t i = 0; i < msg.length(); ++i)
    buf_file_addr[i] = msg[i];
  //buf_file_addr[msg.length()] = '\n';

  std::cout << "buf_file_addr at line 73: "<<buf_file_addr<< std::endl;
  // buf_file_addr is what is sent from the producer, but NEEDS be changed

  // update transfer file
  if (msync(buf_file_addr, buf_size, MS_SYNC) < 0)
    HandleError("Synchronizing transfer file map");;

  // signal consumer filename is transferred
  log_sig_.Up();

  // wait to see if file can be opened
  log_sig_.Down();
  char jo = buf_file_addr[0];
  //char jo = buf_file_addr[5];
  // BUS ERROR
  //std::cout << "buf_file_addr at end "<<buf_file_addr<< std::endl;

  //signal consumer that capitalization is finished
  log_sig_.Up();
  std::cout << "buf_file_addr at end: "<<buf_file_addr<< std::endl;
  return 0;
}

}  // namespace logger
