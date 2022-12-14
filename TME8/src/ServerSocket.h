#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include <sys/socket.h>

#include "Socket.h"

namespace pr {

class ServerSocket {
  int socketfd;
  struct sockaddr_in t;

 public:
  // Demarre l'ecoute sur le port donne
  ServerSocket(int port) {
    int val = 1;
    struct sockaddr_in t;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0) {
      perror("setsockopt");
      exit(1);
    }

    t.sin_addr.s_addr = INADDR_ANY;
    t.sin_family = AF_INET;
    t.sin_port = htons(port);

    if (bind(socketfd, (struct sockaddr*)&t, sizeof(t)) < 0) {
      perror("bind");
      exit(1);
    }

    if (listen(socketfd, SOMAXCONN) < 0) {
      perror("listen");
      exit(1);
    }
  }

  int getFD() { return socketfd; }
  bool isOpen() const { return socketfd != -1; }

  Socket accept() {
    int fd;

    socklen_t s = sizeof(t);
    if ((fd = ::accept(socketfd, (struct sockaddr*)&t, &s)) < 0) {
      perror("accept");
      exit(1);
    }
    std::cout << &t << std::endl;
    return Socket(fd);
  }

  void close() { ::close(socketfd); }
};

}  // namespace pr
#endif /* SRC_SERVERSOCKET_H_ */
