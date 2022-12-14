#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>
#include <vector>

#include "hashmap.hpp"

using namespace std;
using namespace std::chrono;

void print_stat(pr::HashTable<string, int>& h, string s) {
  int* p = h.get(s);
  cout << s << ": " << (p != nullptr ? *p : 0) << "\n";
}

int main() {
  pr::HashTable<string, int> h;

  ifstream input = ifstream("WarAndPeace.txt");

  auto start = steady_clock::now();
  cout << "Parsing War and Peace"
       << "\n";

  size_t nombre_lu = 0;
  // prochain mot lu
  string word;
  // une regex qui reconnait les caractères anormaux (négation des lettres)
  regex re(R"([^a-zA-Z])");
  while (input >> word) {
    // élimine la ponctuation et les caractères spéciaux
    word = regex_replace(word, re, "");
    // passe en lowercase
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    // word est maintenant "tout propre"
    if (nombre_lu % 100 == 0)
      // on affiche un mot "propre" sur 100
      cout << nombre_lu << ": " << word << "\n";

    int* p = h.get(word);
    if (p == nullptr) {
      h.put(word, 0);
    } else {
      *p += 1;
    }
    nombre_lu++;
  }
  input.close();

  cout << "Finished Parsing War and Peace"
       << "\n";

  auto end = steady_clock::now();
  cout << "Parsing took " << duration_cast<milliseconds>(end - start).count()
       << "ms.\n";

  cout << "Found a total of " << nombre_lu << " words."
       << "\n";

  cout << "unique words: " << h.size() << "\n";
  cout << "unique words (count exo 1.1): "
       << pr::HashTable<string, int>::count(h.begin(), h.end()) << "\n";

  print_stat(h, "war");
  print_stat(h, "peace");
  print_stat(h, "toto");

  vector<pair<string, int>> vec;

  for (auto& x : h) vec.push_back(pair<string, int>(x.key, x.value));

  sort(vec.begin(), vec.end(),
       [](const pair<string, int>& a, const pair<string, int>& b) {
         return get<1>(a) > get<1>(b);
       });

  cout << "top 10 words : "
       << "\n";

  for (size_t i = 0; i < 10; ++i) {
    auto& x = vec[i];
    cout << get<0>(x) << ": " << get<1>(x) << "\n";
  }

  return 0;
}
