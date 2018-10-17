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

void get_add(string address, int &a, int &b) {
  vector<string> tokens = split(address, '.');
  a = (stoi(tokens[0]) << 8) | stoi(tokens[1]);
  b = (stoi(tokens[2]) << 8) | stoi(tokens[3]);
}

void add(int &result, int a) {
  int res = result + a;
  if ((res >> 16) == 0) {
    result = res;
    return;
  }
  int carry = res >> 16;
  result = res & 0xffff;
  while (carry) {
    res = result;
    res += carry;
    result = res & 0xffff;
    carry = res >> 16;
  }
}

int main() {
  int type_of_service, tot_len, identifier;
  int dont_fragment, more_fragments;
  int fragment_offset, ttl, protocol;
  string src, dest;
  cout << "Enter:\n Type of Service ";
  cin >> type_of_service;
  cout << "\nTotal Length ";
  cin >> tot_len;
  cout << "\nIdentifier ";
  cin >> identifier;
  cout << "\nDont Fragment(0/1) ";
  cin >> dont_fragment;
  cout << "\nMore Fragments(0/1) ";
  cin >> more_fragments;
  cout << "\nFragment offset ";
  cin >> fragment_offset;
  cout << "\nTTL ";
  cin >> ttl;
  cout << "\nProtocol ";
  cin >> protocol;
  cout << "\nSource ";
  cin >> src;
  cout << "\nDestination ";
  cin >> dest;
  int src1, src2, dest1, dest2;
  get_add(src, src1, src2);
  get_add(dest, dest1, dest2);
  int flags = dont_fragment << 1 | more_fragments;

  int result = 0;
  type_of_service |= 17664;
  add(result, type_of_service);
  cout << hex << type_of_service;
  cout << " " << hex << result; cout << "\n";

  add(result, tot_len);
  cout << hex << tot_len;
  cout << " " << hex << result; cout << "\n";
  
  add(result, identifier);
  cout << hex << identifier;
  cout << " " << hex << result; cout << "\n";

  add(result, (flags<<13) | fragment_offset);
  cout << hex << ((flags<<13) | fragment_offset);
  cout << " " << hex << result; cout  << "\n";
  
  add(result, (ttl<<8) | protocol);
  cout << hex << ((ttl<<8) | protocol);
  cout << " " << hex << result; cout << "\n";

  add(result, src1);
  cout << hex << src1; cout  << " " << hex << result; cout << "\n";

  add(result, src2);
  cout << hex << src2; cout << " " << hex << result; cout << "\n";

  add(result, dest1);
  cout << hex << dest1; cout << " " << hex << result; cout << "\n";

  add(result, dest2);
  cout << hex << dest2; cout << " " << hex << result; cout << "\n";
  
  cout << "HEC = " << hex << ~result; cout << "\n";
  return 0;
}
