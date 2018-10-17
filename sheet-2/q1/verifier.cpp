#include <iostream>
#include <string>

using namespace std;

bool check(string msg, string poly) {
  int k = poly.length() - 1;
  string remainder;
  for (int i = 0; i < k; i++) {
    remainder += "0";
  }
  int dividend = stoi(msg + remainder, nullptr, 2);
  int divisor = stoi(poly, nullptr, 2);
  
  while(dividend >> k) {
    int a = divisor;
    while((a << 1) < dividend) {
      a <<= 1;
    }
    dividend ^= a;
  }
  return dividend == 0;
}


int main() {
  string msg, poly;
  getline(cin, msg, '\n');
  getline(cin, poly, '\n');
  if (check(msg, poly)) {
    cout << "Correct\n";
  } else {
    cout << "Incorrect\n";
  }
  return 0;
}
