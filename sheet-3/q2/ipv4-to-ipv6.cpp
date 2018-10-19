#define _GLIBCXX_USE_CXX11_ABI 0
#include <bits/stdc++.h>

using namespace std;

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int main() {
  string ipv4;
  cout << "Enter IPv4 addr: ";
  cin >> ipv4;
  vector<string> v;
  v = split(ipv4, '.');
  printf("%02x%02x:%02x%02x\n", stoi(v[0]), stoi(v[1]), stoi(v[2]), stoi(v[3]));
  return 0;
}
