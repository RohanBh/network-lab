#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <thread>

#define SERV_PORT 8080
#define MAX_GROUP_CAPACITY 3

using namespace std;

int main(int argc, char **argv) {
  
  if (argc <= 2) {
    cout << "Usage: chat_server server_name chat_room1 chat_room2 ...\n";
    return -1;
  }
  
  int ls;
  vector<thread*> clients;
  vector<int> dataSockets;
  int client_id = -1;
  struct sockaddr_in servAddr = {};
  struct sockaddr_in clientAddr = {};
  socklen_t clientAddrLen;

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(SERV_PORT);

  string serverName = argv[1];
  vector<string> chatRooms;
  for (int i = 2; i < argc; i++) {
    chatRooms.push_back(argv[i]);
  }
  string welcomeMessage = string("Welcome to ") + serverName + string(" chat server. Please choose a room.\n");
  for (int i = 0; i < chatRooms.size(); i++) {
    string room = to_string(i + 1) + ". " + chatRooms[i] + "\n";
    welcomeMessage += room;
  }
  vector<vector<string> > msgQueue;
  for (int i = 0; i < chatRooms.size(); i++) {
    vector<string> messages;
    string welcome = "You have joined room " + chatRooms[i] + "\n";
    messages.push_back(string(welcome.size(), '-') + "\n");
    messages.push_back(welcome);
    messages.push_back(string(welcome.size(), '-') + "\n");
    msgQueue.push_back(messages);
  }
  
  if ((ls = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "listen socket creation failed!" << "\n";
    return -1;
  }
  int opt = 1;
  if (setsockopt(ls, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) { 
    cout << "set socket options failed!\n";
    return -1;
  }
  if (bind(ls, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) {
    cout << "listen socket binding failed!\n";
    return -1;
  }
  if (listen(ls, 5) < 0) {
    cout << "Could not complete connection establishment phase!\n";
    return -1;
  }

  while (true) {
    int dataSocket;
    if ((dataSocket = accept(ls, (struct sockaddr*) &clientAddr, &clientAddrLen)) < 0) {
      cout << "Could not accept client!\n";
      continue;
    }
    dataSockets.push_back(dataSocket);
    client_id++;
    auto f = [&]() mutable {
      int my_id = client_id;
      int s = dataSockets[my_id];
      int n;
      char buffer[1024] = {0};
      int i = 0;
      n = recv(s, buffer, 10, 0);
      buffer[n] = '\0';
      string name = buffer;
      cout << "Client connected: " << name << "\n";
      send(s, welcomeMessage.c_str(), welcomeMessage.length(), 0);
      n = recv(s, buffer, 1, 0);
      buffer[n] = '\0';
      int roomNo = atoi(buffer) - 1;
      cout << name << " chose room " << chatRooms[roomNo] << "\n";
      msgQueue[roomNo].push_back("** " + name + " has joined the chat **\n");
      while (true) {
	while (i < msgQueue[roomNo].size()) {
	  send(s, msgQueue[roomNo][i].c_str(), msgQueue[roomNo][i].length(), 0);
	  i++;
	}
	n = recv(s, buffer, 1024, MSG_DONTWAIT);
	if (n == -1) n = 0;
	buffer[n] = '\0';
	if (n != 0) {
	  msgQueue[roomNo].push_back(name + ": " + buffer + "\n");
	  i++;
	}
      }
    };
    clients.push_back(new thread(f));
  }

  for (int s: dataSockets) {
    close(s);
  }
  return 0;
}
