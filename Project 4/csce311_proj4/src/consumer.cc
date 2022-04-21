// Copyright 2022 CSCE 311
//

#include <csce311_proj4/inc/consumer.h>
#include <iostream>
#include <string>
#include <fstream>
using std::cout;
using std::endl;

namespace logger {

Consumer::Consumer() : log_sig_(log_sig_name_) {
  // create signal mux (unlocked by producer(s))
  log_sig_.Create(0);
  log_sig_.Open();
}


Consumer::~Consumer() {
  // delete named semaphore
  log_sig_.Destroy();

  ::exit(0);
}


std::tuple<int, off_t> Consumer::OpenFile(const char file_name[], int flags) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);
  if (fd < 0) {
    std::cerr << ::strerror(errno) << std::endl;
    log_sig_.Destroy();
    ::exit(errno);
  }

  // get size of file
  struct ::stat f_stats;
  if (::fstat(fd, &f_stats) < 0)
    HandleError("OpenFile");

  return {fd, f_stats.st_size};
}


void Consumer::HandleError(const char msg[]) {
  std::cerr << "Source: " << msg << std::endl;
  std::cerr << '\t' << ::strerror(errno) << std::endl;
  log_sig_.Destroy();
  ::exit(errno);
}


void Consumer::Consume(const char log_file_name[]) {
  while (true) {
    log_sig_.Down();  // block until client ready
    cout << "CLIENT REQUEST RECEIVED"<< endl;

    // open transfer file and get size
    int buf_fd;
    long int buf_size;  // off_t is a long int
    std::tie(buf_fd, buf_size) = OpenFile(buf_file_name_, O_RDWR);

    // map to transfer file pages in page cache
    char *buf_file_addr = static_cast<char *>(
      ::mmap(nullptr, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, buf_fd, 0));

      //testing
      std::cout << "buf_file_addr at line 64: "<<buf_file_addr<<std::endl;
      // need to remove the newline symbol from buf_file_addr
    if (buf_file_addr == MAP_FAILED)
      HandleError("Transfer file map");
    if (::close(buf_fd) < 0)
      HandleError("Transfer file map close");
    // open log file and get size
    // remove the newline from buf_file_addr
    char* tempchar = buf_file_addr;
    tempchar[strlen(tempchar)-1] = '\0';

    int log_fd;
    long int log_size;  // off_t is a long int
    std::tie(log_fd, log_size) = OpenFile(tempchar, O_RDWR);
    tempchar[strlen(tempchar)-1] = '\n';
    // add necessary bytes to end of log file
    if (::fallocate(log_fd, 0, log_size, buf_size) < 0)
      HandleError("Allocating additional log file size");

    // map to log file pages in page cache and allocate additional pages
    char *log_file_addr = static_cast<char *>(::mmap(nullptr,
                                                     log_size + buf_size,
                                                     PROT_READ | PROT_WRITE,
                                                     MAP_SHARED,
                                                     log_fd,
                                                     0));

    //std::cout << "log_file_addr at line 82 "<<log_file_addr<< std::endl;
    // log_file_addr is what is inside the file
    if (log_file_addr == MAP_FAILED)
      HandleError("Log file map");
    if (::close(log_fd) < 0)
      HandleError("Log file map close");

    //std::cout << "log size: " << log_size<<std::endl;
    //std::cout << "buf suze: " << buf_size<<std::endl;
    //buf_file_addr = log_file_addr; // this works, but is unneeded
    /*
    // adds buf_file_addr to top of log file
    for (long int i = 0; i < buf_size; ++i) {
      log_file_addr[log_size+i] = buf_file_addr[i];
    }
    */

    //log_sig_.Up();// signal to producer that buf_file_addr is ready to go


    //testing
    //std::cout << "log_file_addr at line 94 "<<log_file_addr<<endl;
    // log file is what's in log file + buf_file_addr
    std::cout << "buf_file_addr at line 94: "<<buf_file_addr<<std::endl;
    // buf_file_addr is what is sent from producer

    //wait for producer to finish changing to uppercase
    //log_sig_.Down();

    // update log file
    if (msync(log_file_addr, log_size + buf_size, MS_SYNC) < 0)
      HandleError("Synchronizing log file map");

    // empty transfer file
    if (::truncate(buf_file_name_, 0) < 0)
      HandleError("Emptying transfer file");

    // release copy of mapped mem
    if (::munmap(buf_file_addr, buf_size))
      HandleError("Buffer file unmap");
    if (::munmap(log_file_addr, log_size + buf_size))
      HandleError("Log file unmap");


    //signal producer that the file is ready
    log_sig_.Up();

    // copy from transfer file to log file-
    /*
    for (long int i = 0; i < buf_size; ++i) {
      log_file_addr[log_size + i] = buf_file_addr[i];
    }
    // update log file
    if (msync(log_file_addr, log_size + buf_size, MS_SYNC) < 0)
      HandleError("Synchronizing log file map");

    // empty transfer file
    if (::truncate(buf_file_name_, 0) < 0)
      HandleError("Emptying transfer file");
    */
    break; // for testing
  }
}


}  // namespace logger

/*
things to note: buf_file_addr is the shared mem space: it gets tranferred from one array to another in both files
*/
