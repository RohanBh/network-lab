#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
  int a = fork();
  if (a) {
    cout << "Child Pid: " << a << "\n";
  } else {
    cout << "Parent Pid: " << getppid() << "\n";
  }
  return 0;
}
