#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

using namespace std;

using persona = pair<string, bool>;
using dic = map<int, persona>;
using conj = set<string>;

int main()
{
	int E, S;	cin >> E >> S;

	while (E != -1 && S != -1) {

		dic listaEntradas;
		conj dentroHab;

		int minTiempo = 0, maxTiempo = 0;

		for (int i = 0; i < E; i++) {
			int instante;	string nombre;
			cin >> instante >> nombre;
			
			listaEntradas[instante] = persona(nombre, true);
		}

		for (int i = 0; i < S; i++) {
			int instante;	string nombre;
			cin >> instante >> nombre;

			listaEntradas[instante] = persona(nombre, false);
		}

		bool correcto = true;
		int maxGente = 0;
		dic::iterator it = listaEntradas.begin();
		while (correcto && it != listaEntradas.end())
		{
			conj::iterator elem = dentroHab.find((*it).second.first);
			persona p = (*it).second;

			if (p.second) {	// entrada
				if (elem != dentroHab.end()) {
					correcto = false;
				}
				else {
					dentroHab.insert(p.first);	// mete la persona p
				}
			}
			else {			// salida
				if (elem == dentroHab.end()) {
					correcto = false;
				}
				else {
					dentroHab.erase(p.first);	// saca la persona p
				}
			}
			if (correcto && dentroHab.size() > maxGente) {
				maxGente = dentroHab.size();
			}

			++it;
		}
		if (!correcto)
			cout << "ERROR" << endl;
		else
			cout << maxGente << endl;

		cin >> E >> S;
	}
	return 0;
}