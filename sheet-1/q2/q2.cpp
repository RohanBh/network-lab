#include <iostream>
using namespace std;

int main(int argc, char** argv) {
  int a, b;
  char op;
  a = atoi(argv[1]);
  b = atoi(argv[3]);
  op = argv[2][0];
  switch(op) {
  case '+': cout << a + b;
    break;
  case '-': cout << a - b;
    break;
  case '*': cout << a * b;
    break;
  case '/': cout << a / b;
    break;
  }
  cout << "\n";
  return 0;
}
