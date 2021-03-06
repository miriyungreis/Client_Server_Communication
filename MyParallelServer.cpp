//
// Created by miri on 23.1.2020.
//

#include "Server.h"
#include <iterator>

using namespace server_side;
using namespace std;

/**
 *
 * @param port
 * @param c
 */
void MyParallelServer::open(int port, ClientHandler *c) {
  unsigned short int portl = port;
  //create socket
  struct sockaddr_in serv_address{};
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    cerr << "couldnot create socket" << endl;
    exit(1);
  }
  bzero((char *) &serv_address, sizeof(serv_address));
  serv_address.sin_family = AF_INET;
  serv_address.sin_addr.s_addr = INADDR_ANY;
  serv_address.sin_port = htons(portl);
  if (bind(socketfd, (struct sockaddr *) &serv_address, sizeof(serv_address))
      == -1) {
    cerr << "couldnot bind the socket to an IP" << endl;
    exit(1);
  }
  //start listening
  if (listen(socketfd, 5) == -1) {
    cerr << "eror during listening to port" << endl;
  } else {
    cout << "listening to port" << endl;
  }
  //accept clients
  acceptClients(socketfd, c);
}

/**
 *
 * @param socket
 * @param client_handler
 */
void MyParallelServer::acceptClients(int socket,
                                     ClientHandler *client_handler) {
  //first client
  struct sockaddr_in client_adress{};
  int timeout_in_seconds = 120;
  struct timeval tv;
  tv.tv_sec = timeout_in_seconds;
  setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof(tv));
  int client_length = sizeof(client_adress);
  //accept client
  int client_socket = accept(socket,
                             (struct sockaddr *) &client_adress,
                             (socklen_t *) &client_length);
  if (client_socket < 0) {
    cerr << "no client found\nDone\n" << endl;
    //stop(socket);
    return;
  }
  ClientHandler *temp_handler = client_handler->deepCopy();
  this->m_threads.push_back(thread(&MyParallelServer::HandleClientAdapter,
                                   this,
                                   temp_handler,
                                   client_socket));
  //this->m_threads.front().join();
  timeout_in_seconds = 120;
  tv.tv_sec = timeout_in_seconds;
  tv.tv_usec = 0;
  setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof(tv));
  //all clients
  while (1) {
    //one second for each client
    //accept client
    int client_socket = accept(socket,
                               (struct sockaddr *) &client_adress,
                               (socklen_t *) &client_length);
    if (client_socket < 0) {
      cerr << "no client found\nDone\n" << endl;
      //stop(socket);
      break;
    }
    ClientHandler *temp_handler = client_handler->deepCopy();
    //thread id(&MyParallelServer::HandleClientAdapter,this, temp_handler, socket);
    this->m_threads.push_back(thread(&MyParallelServer::HandleClientAdapter,
                                     this,
                                     temp_handler,
                                     client_socket));
  }
  this->stop(client_socket);
  this->stop(socket);
  //join
  join_threads();
};

/**
 *
 * @param c
 * @param socket
 * @return
 */
void *MyParallelServer::HandleClientAdapter(ClientHandler *c, int socket) {
  c->HandleClient(socket, socket);
}

/**
 *
 */
void MyParallelServer::join_threads() {
  while (!(this->m_threads.empty())) {
    this->m_threads.front().join();
    this->m_threads.pop_front();
  }
}

/**
 *
 * @param socket
 */
void MyParallelServer::stop(int socket) {
  close(socket);
}

