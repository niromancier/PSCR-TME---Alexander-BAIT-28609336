#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iosfwd>
#include <iostream>
#include <ostream>
#include <string>

namespace pr {

inline std::ostream& operator<<(std::ostream& os,
                                const struct sockaddr_in* addr) {
  char hostnane[1024] = {0};
  if (!getnameinfo((struct sockaddr*)addr, sizeof(*addr), hostnane, 1024,
                   nullptr, 0, 0)) {
    os << "[" << hostnane << "] ";
  }
  return os << inet_ntoa(addr->sin_addr) << ":" << ntohs(addr->sin_port);
}

class Socket {
  int fd;

 public:
  Socket() : fd(-1) {}
  Socket(int fd) : fd(fd) {}

  void connect(const std::string& host, int port) {
    struct addrinfo* addr;
    if (getaddrinfo(host.c_str(), nullptr, nullptr, &addr) < 0) {
      perror("getaddrinfo");
      exit(1);
    }
    in_addr ipv4 = ((struct sockaddr_in*)addr->ai_addr)->sin_addr;
    freeaddrinfo(addr);
    connect(ipv4, port);
  }

  void connect(in_addr ipv4, int port) {
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    struct sockaddr_in t;
    t.sin_addr = ipv4;
    t.sin_family = AF_INET;
    t.sin_port = htons(port);

    std::cout << &t << std::endl;

    if (::connect(fd, (struct sockaddr*)&t, sizeof(t)) < 0) {
      perror("connect");
      exit(1);
    }
  }

  bool isOpen() const { return fd != -1; }
  int getFD() { return fd; }
  void close() { ::close(fd); };
};

}  // namespace pr

#endif /* SRC_SOCKET_H_ */
