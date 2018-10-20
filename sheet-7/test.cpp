#include <iostream>
#include <bits/stdc++.h>
#include <regex>

using namespace std;

int main()
{
  // (\\w+://)?[wW]{3}\\.\\w+\\.[a-zA-Z]+(:\\d+)?(/\\w+)*/?"
  regex a(".+?;");
  smatch m;
  string b;
  cin >> b;
  regex_search(b, m, a);
  cout << m.str(0) << "\n";
  return 0;
}
