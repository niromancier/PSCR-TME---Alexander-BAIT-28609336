#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("/home/etudiant/workspacecpp/tme2s/src/WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	vector<string>v;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	int i=0;
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		for (i=0;i<v.size();i++ ){
			if(word==v[i]){
				break;
			}
		}

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		if(i==0){
			v.push_back(word);
			nombre_lu++;
			continue;
		}
		if(word==v[i]){
			continue;
		}
		v.push_back(word);
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    return 0;
}


