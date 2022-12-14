#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <unordered_map>

#define BUF_SIZE 128

namespace pr {
  inline bool parse(char* buf, char** b, char **dd) {
    char* d = buf += 2;
    while (*d) {
      if (*d == ' ') break; 
      d++;
    }
    *d = '\0';
    d++;
    *b = buf;
    *dd = d;
    return true;
  }

  class UDPserver {
    int port_;
    int s_;
    std::unordered_map<std::string, std::string> data;
   public:
    UDPserver(int port) : port_(port) {}

    ~UDPserver() { data.clear(); }

    bool startServer() {
      if ((s_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return false;
      }

      struct sockaddr_in addr, client;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = INADDR_ANY;
      addr.sin_port = htons(port_);

      int opt = 1;
      setsockopt(s_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int));

      if (bind(s_, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return false;
      }

      socklen_t client_size = sizeof(client);
      char *b = NULL, *d = NULL;
      while (true) {
        char buffer[BUF_SIZE];
        std::memset(buffer, 0, BUF_SIZE);
        int n = recvfrom(s_, buffer, BUF_SIZE, MSG_WAITALL, (struct sockaddr *)&client, &client_size);
        buffer[n] = '\0';
        if (!parse(buffer, &b, &d)) return false;
        switch (buffer[0]) {
          case 'S': {
            data[b] = d;
            sendto(s_, (const char *)"saved", 5, MSG_CONFIRM, (const struct sockaddr *)&client, client_size);
            break;
          }
          case 'G': {
            std::string rep = data[b];
            if (rep.empty()) {
              rep = "empty";
            }
            sendto(s_, rep.c_str(), rep.length(), MSG_CONFIRM, (const struct sockaddr *)&client, client_size);
            break;
          }
          default:
            return false;
        }
      }
      return true;
    }
  };
}
