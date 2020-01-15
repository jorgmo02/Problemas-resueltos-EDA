#include <iostream>
#include <string>
#include <map>

using namespace std;
using dic = map<string, int>;

int main()
{
	size_t interacciones, maximo;
	cin >> interacciones >> maximo;

	while (interacciones != 0) {
		dic objetos;
		string elemento;
		for (int i = 0; i < interacciones; i++) {
			string op = "";		cin >> op;
			if (op == "ADD") {		// añade una unidad de elemento
				cin >> elemento;
				if(objetos[elemento] < maximo) objetos[elemento]++;
			}
			else if (op == "USE") {	// gasta una unidad de elemento (siempre queda al menos 1)
				string elto; cin >> elto;
				objetos[elto]--;
			}
			else {					// suma 5 al último elemento añadido
				string aux;	cin >> aux;
				objetos[elemento] += 5;
				if (objetos[elemento] > maximo) objetos[elemento] = 0;
			}
		}

		for (dic::iterator i = objetos.begin(); i != objetos.end(); ++i)
			cout << (*i).first << " " << (*i).second << endl;

		cout << endl;
		cin >> interacciones >> maximo;
	}
	return 0;
}