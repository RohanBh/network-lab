#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <thread>
#define SERV_PORT 8080

using namespace std;
  
int main(int argc, char const *argv[]) {
  if (argc == 1) {
    cout << "Usage: ./chat_client user_name\n";
    return -1;
  }
  int s = 0;
  int n;
  struct sockaddr_in clientAddr = {};
  struct sockaddr_in servAddr = {};
  string user = argv[1];

  char buffer[1024] = {0};
  
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not create socket!\n";
    return -1;
  }
  
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(SERV_PORT);
  
  if (connect(s,(struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
    cout << "Connection to server failed!\n";
    return -1;
  }
  send(s, user.c_str(), user.length(), 0);
  n = recv(s, buffer, 1024, 0);
  buffer[n] = '\0';
  cout << buffer;
  string choice;
  cout << "Choice: ";
  getline(cin, choice);
  send(s , choice.c_str() , choice.length(), 0);
  
  thread user_thread([&]() mutable {
      string message;
      cout << "You: ";
      while (getline(cin, message)) {
	send(s, message.c_str(), message.length(), 0);
	cout << "You: ";
      }
    });
  thread server_thread([&]() mutable {
      while (true) {
	n = recv(s, buffer, 1024, 0);
	buffer[n] = '\0';
	cout << "\b\b\b\b\b";
	cout << buffer;
	cout << "You: ";
	cout.flush();
      }
    });
  user_thread.join();
  close(s);
  
  return 0;
}
