/**
 * @author huahang.liu@gmail.com
 * @date 2015-10-12
 */

#include <iostream>
#include <thread>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

void waitAndClose(int fd) {
  sleep(10);
  int rtn = close(fd);
  if (0 != rtn) {
    std::cerr << "close() failed" << std::endl;
    exit(1);
  }
  std::cerr << "Closed" << std::endl;
}

int main() {
  int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    std::cerr << "socket() failed with code: " << serverSocket << std::endl;
    return 1;
  }
  sockaddr_un serverSocketAddress;
  serverSocketAddress.sun_family = AF_UNIX;
  strcpy(serverSocketAddress.sun_path, "/tmp/accept_interruption_test_sock");
  int rtn = bind(
    serverSocket,
    (sockaddr*) &serverSocketAddress,
    sizeof(sockaddr_un)
  );
  if (0 != rtn) {
    std::cerr << "bind() failed" << std::endl;
    return 1;
  }
  rtn = listen(serverSocket, 128);
  if (0 != rtn) {
    std::cerr << "listen() failed" << std::endl;
    return 1;
  }
  std::thread closeThread(waitAndClose, serverSocket);
  while(true) {
    sockaddr_un clientSocketAddress;
    socklen_t clientSocketAddressSize = sizeof(sockaddr_un);
    int clientSocket = accept(
      serverSocket,
      (sockaddr*) &clientSocketAddress,
      &clientSocketAddressSize
    );
    if (clientSocket < 0) {
      std::cerr << "accept() failed" << std::endl;
      break;
    }
    std::string response = "Bye-bye!\n";
    rtn = write(clientSocket, response.data(), response.length());
    if (rtn != response.length()) {
      std::cerr << "write() failed" << std::endl;
      return 1;
    }
    rtn = close(clientSocket);
    if (0 != rtn) {
      std::cerr << "close() failed" << std::endl;
      return 1;
    }
  }
  closeThread.join();
  return 0;
}
