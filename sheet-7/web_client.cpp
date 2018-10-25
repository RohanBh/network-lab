#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <regex>
#include <ctime>

#define pss pair <string, string>

using namespace std;

bool parse(string URL, string &remote, int &port, string &file) {
  regex urlRegex("(\\w+://)?([a-z]+\\.)?(\\w+\\.)+[a-zA-Z]+(:\\d+)?(/[\\d\\w\\$\\-_\\!~\\+]+)+\\.\\w+");
  if (!regex_match(URL, urlRegex)) {
    cout << "Bad URL: " << URL << "\n";
    return false;
  }
  regex hostnameRegex("([a-z]+\\.)?(\\w+\\.)+[a-zA-Z]+");
  smatch matcher;
  regex_search(URL, matcher, hostnameRegex);
  remote = matcher.str(0);
  regex portRegex(":\\d+");
  port = 80;
  regex_search(URL, matcher, portRegex);
  if (!matcher.str(0).empty()) {
    port = stoi((matcher.str(0).c_str()) + 1);
  }
  regex fileRegex("((/[\\d\\w\\$\\-_!~\\+]+)+\\.\\w+)$");
  regex_search(URL, matcher, fileRegex);
  file = matcher.str(0);
  return true;
}

// See: https://askubuntu.com/a/16583/672640; https://specifications.freedesktop.org/desktop-entry-spec/0.9.5/ar01s07.html
bool openFile(string file, string mime) {
  ifstream mimefile("/usr/share/applications/mimeinfo.cache");
  if (!mimefile.is_open()) return false;
  string line;
  regex appRegex("([^;]+;)$");
  smatch matcher;
  string desktopEntry;
  while (getline(mimefile, line)) {
    if (line.find(mime) == string::npos) continue;
    regex_search(line, matcher, appRegex);
    string str = matcher.str(0);
    desktopEntry = str.substr(0, str.length() - 1);
    break;
  }
  if (desktopEntry.empty()) return false;
  mimefile.close();
  
  ifstream desktopfile("/usr/share/applications/" + desktopEntry);
  string execName;
  if (!desktopfile.is_open()) return false;
  while (getline(desktopfile, line)) {
    if (line.substr(0, 5) != "Exec=") continue;
    regex app("=\\S*");
    regex_search(line, matcher, app);
    string str = matcher.str(0);
    execName = str.substr(1);
    break;
  }
  execName = execName;
  char *exec_cstr = new char[execName.length() + 1];
  strcpy(exec_cstr, execName.c_str());
  char *file_cstr = new char[file.length() + 1];
  strcpy(file_cstr, file.c_str());
  char *args[] = {exec_cstr, file_cstr, NULL};
  execvp(args[0], args);
  return true;
}

bool readCache(map <string, pss > &cache) {
  ifstream cachefile(".cache/index");
  if (!cachefile.is_open()) return false;
  string line;
  while (getline(cachefile, line)) {
    int a = line.find(',');
    int b = line.find(',', a + 1);
    if (a == string::npos || b == string::npos) return false;
    cache.insert(pair <string, pss > (
			       line.substr(0, a),
			       pss (line.substr(a + 1, b - a - 1), line.substr(b + 1))
				      ));
  }
  return true;
}

bool writeCache(map <string, pss > &cache) {
  ofstream cachefile(".cache/index");
  if (!cachefile.is_open()) return false;
  for (auto &kv: cache) {
    cachefile << kv.first << "," << kv.second.first << "," << kv.second.second << "\n";
  }
  return true;
}
  
int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: web_browser URL\n";
    return -1;
  }

  string URL, host, file;
  int port;
  URL = argv[1];
  if (parse(URL, host, port, file)) {
    printf("---------------\nURL: %s\nHost: %s\nPort: %d\nFile: %s\n---------------\n\n",
	   URL.c_str(), host.c_str(), port, file.c_str());
  } else {
    return -1;
  }

  map <string, pss > cache;
  readCache(cache);

  auto it = cache.find(URL);
  if (it != cache.end()) {
    cout << "Found cached Response!\nOpening Default Application...\n";
    if (!openFile(it->second.first, it->second.second)) {
      cout << "Error opening file " << tmpfile;
    }
    return -1;
  }
  
  int s = 0; // client's socket descriptor
  int n = 1; // number of bytes recv from server in one call to recv
  struct sockaddr_in clientAddr = {};
  struct sockaddr_in servAddr = {};

  char buffer[1024] = {0};
    
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not create socket!\n";
    return -1;
  }
    
  servAddr.sin_family = AF_INET;
  struct hostent *host_entry;
  host_entry =  gethostbyname(host.c_str());
  servAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
  servAddr.sin_port = htons(port);
  
  if (connect(s,(struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
    cout << "Connection to server failed!\n";
    return -1;
  }

  // Use CRLF line ending: https://stackoverflow.com/a/5757349
  // Mention Host in request header -
  // https://www.howtogeek.com/254097/how-do-web-servers-know-if-you-are-using-direct-ip-address-access-or-not/
  string message = "GET " + file + " HTTP/1.0\r\n" + "Host: " + host + "\r\n\r\n";
  time_t seconds; // represents the current time
  time(&seconds);
  string tmpfile = ".cache/tmp" + to_string(seconds);
  ofstream outfile(tmpfile, ios::out | ios::trunc);
  bool mimeflag = false;
  bool contentflag = false;
  regex mimeRegex("Content\\-Type: .+?(;|\\r\\n)");
  cmatch matcher;
  string mimeType;
  
  cout << "Sending request...\n" << message;
  send(s , message.c_str() , strlen(message.c_str()), 0);
  while (n > 0) {
    n = recv(s, buffer, 1024, 0);
    if (!mimeflag) {
      regex_search(buffer, matcher, mimeRegex);
      string str = matcher.str(0);
      if (!str.empty()) {
	mimeType = str.substr(14, str.length() - 14);
	if (mimeType[mimeType.length() - 1] == ';') {
	  mimeType = mimeType.substr(0, mimeType.length() - 1);
	} else {
	  mimeType = mimeType.substr(0, mimeType.length() - 2);
	}
	mimeflag = true;
      }
    }
    if (!contentflag) {
      char *str = strstr(buffer, "\r\n\r\n");
      if (str != NULL) {
	str = str + 4;
	outfile.write(str, n - (str - buffer));
	contentflag = true;
      }
    } else {
      outfile.write(buffer, n);
    }
  }
  close(s);
  outfile.close();
  if (mimeType == "text/html") {
    rename(tmpfile.c_str(), string(tmpfile + ".html").c_str());
    tmpfile += ".html";
  }
  cache.insert(pair <string, pss > (URL, pss(tmpfile, mimeType)));
  writeCache(cache);
  cout << "\b\bRequest Successful!!!\n" << "\nMime Type: " << mimeType << "\nOpening Default application...\n\n";
  if (!openFile(tmpfile, mimeType)) {
    cout << "Error opening file " << tmpfile << "\n";
  }
  return -1;
}
