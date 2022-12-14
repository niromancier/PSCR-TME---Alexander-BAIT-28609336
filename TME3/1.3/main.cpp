#include <algorithm>
#include <chrono>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
using namespace std::chrono;

void print_stat(unordered_map<string, int>& h, string s) {
  auto it = h.find(s);
  int& p = get<1>(*it);
  cout << s << ": " << (it != h.end() ? p : 0) << "\n";
}

int main() {
  unordered_map<string, int> h;

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

    h[word]++;
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
  cout << "unique words (count exo 1.1): " << std::distance(h.begin(), h.end())
       << "\n";

  print_stat(h, "war");
  print_stat(h, "peace");
  print_stat(h, "toto");

  vector<pair<string, int>> vec;

  for (auto& x : h) vec.push_back(x);

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

  unordered_map<int, forward_list<string>> f;

  for (auto p : h) {
    int n = get<1>(p);
    auto it = f.find(n);
    if (it == f.end()) {
      forward_list<string> l;
      l.push_front(get<0>(p));

      f.insert(make_pair(n, l));
    } else {
      auto& x = get<1>(*it);
      x.push_front(get<0>(p));
    }
  }

  for (auto p : f) {
    int n = get<0>(p);
    if (n == 116 || n == 212) {  // N
      cout << "N = " << n << "\n";
      for (auto x : get<1>(p)) cout << "\t" << x << "\n";
    }
  }

  return 0;
}
