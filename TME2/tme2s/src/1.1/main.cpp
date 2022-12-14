#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>
#include <vector>

using namespace std;
using namespace std::chrono;

void print_stat(const vector<pair<string, int>>& vec, string s) {
  auto it = find_if(vec.begin(), vec.end(), [&s](const auto p) {
    return get<0>(p) == s;
  });
  cout << s << ": " << ((it != vec.end()) ? it->second : it->second) << "\n";
}

int main() {
  vector<pair<string, int>> vec;

  ifstream input = ifstream("WarAndPeace.txt");

  auto start = steady_clock::now();
  cout << "Parsing War and Peace" << "\n";

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

    auto p = find_if(vec.begin(), vec.end(), [&word](const auto& p) {
      return get<0>(p) == word;
    });

    if (p == vec.end()) {
      vec.push_back(pair<string, int>(word, 0));
    } else {
      p->second++;
    }
    nombre_lu++;
  }
  input.close();

  cout << "Finished Parsing War and Peace" << "\n";

  auto end = steady_clock::now();
  cout << "Parsing took " << duration_cast<milliseconds>(end - start).count()
       << "ms.\n";

  cout << "Found a total of " << nombre_lu << " words." << "\n";

  cout << "unique words: " << vec.size() << "\n";

  print_stat(vec, "war");
  print_stat(vec, "peace");
  print_stat(vec, "toto");

  return 0;
}
