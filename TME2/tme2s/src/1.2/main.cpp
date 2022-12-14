#include <iostream>
#include <string>

#include "hashmap.hpp"

int main() {
  pr::HashTable<int, int> x;

  for (size_t i = 0; i < 400; ++i) {
    x.put(i, i);
  }
  std::cout << x.size() << std::endl;
  return 0;
}
