#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include <vector>
#include <signal.h>

#include "ConnectionHandler.h"
#include "ServerSocket.h"

namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
  ServerSocket* ss;            // la socket d'attente si elle est instanciee
  ConnectionHandler* handler;  // le gestionnaire de session passe a la constru
                               // a completer
  std::vector<std::thread> threads;
  std::thread* main;

 public:
  TCPServer(ConnectionHandler* handler) : ss(nullptr), handler(handler) {}
  // Tente de creer une socket d'attente sur le port donné
  bool startServer(int port) {
    thread_local volatile sig_atomic_t sig = false;
    signal(SIGINT, [](int) { sig = true; });
    main = new std::thread([&] {
      ss = new ServerSocket(port);
      while (!sig) {
        threads.emplace_back(
            &ConnectionHandler::handleConnection,
            handler->clone(), 
            ss->accept()
        );
      }
    });
    return true;
  }

  // stoppe le serveur
  void stopServer() {
    pthread_kill(main->native_handle(), SIGINT);
    ss->close();
  }

  ~TCPServer() {
    for (auto& t : threads) t.join();
    delete handler;
    delete ss;
  }
};

}  // namespace pr

#endif /* SRC_TCPSERVER_H_ */
