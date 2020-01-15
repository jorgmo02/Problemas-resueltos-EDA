#include <iostream>
#include <string>

using namespace std;

void vuelta_atras(const string& base, const int baseNum, int colmillo, bool& solEncontrada, bool* used);
bool vampiro(int base);
bool es_solucion(int base, int colmillo);
bool noPierdeSigJugada(int base, int colmillo);
int numCifras(int base);
bool sonColmillo(int baseNum, int colmillo2, bool* used);

void dibuja(bool* used, int len) {
	for (int i = 0; i < len; i++)
	{
		cout << used[i] << " ";
	}
	cout << endl;
}

int numCifras(int base) {
	int i = 0;
	while (base > 0) {
		base /= 10;
		i++;
	}
	return i;
}

bool sonColmillo(int baseNum, int colmillo2, bool* used) {

	string base = to_string(baseNum);
	string c2 = to_string(colmillo2);

	int len = base.length();

	bool* copiaUsed = new bool[len];
	for (int i = 0; i < len; i++)
		copiaUsed[i] = used[i];

	bool correcto = true;
	int i = 0;
	while (correcto && i < c2.length()) {
		int k = 0;
		bool cont = true;
		while (cont && k < len) {
			if (base[k] == c2[i] && !copiaUsed[k]) {
				copiaUsed[k] = true;
				cont = false;
			}
			else k++;
		}
		if (k >= len || !cont)
			correcto = false;
		else i++;
	}

	return correcto;
}

bool noPierdeSigJugada(int base, int colmillo) {
	return colmillo > 0 && numCifras(base) / numCifras(colmillo) >= 2;
}

bool es_solucion(int base, int colmillo, bool* used) {
	if (colmillo > 0) {
		int colmillo2 = base / colmillo;
		int cifrasBase = numCifras(base);

		return colmillo2 * colmillo == base &&
			base % colmillo == 0 &&
			numCifras(colmillo) == numCifras(colmillo2) &&
			(colmillo % 10 != 0 || colmillo2 % 10 != 0) &&
			cifrasBase / numCifras(colmillo) == 2 &&
			cifrasBase / numCifras(colmillo2) == 2 &&
			sonColmillo(base, colmillo2, used);
	}
	else return false;
}

void vuelta_atras(const string& base, const int baseNum, int colmillo, bool& solEncontrada, bool* used) {

	int cifrasBase = numCifras(baseNum);
	int act = 0;
	while (!solEncontrada && act < cifrasBase) {

		if (!used[act]) {

			used[act] = true;
			colmillo = (colmillo * 10) + (base[act] - '0');

			if (es_solucion(baseNum, colmillo, used)) {
				solEncontrada = true;
			}

			else if (noPierdeSigJugada(baseNum, colmillo)) {
				vuelta_atras(base, baseNum, colmillo, solEncontrada, used);
			}

			colmillo /= 10;
			used[act] = false;
		}

		act++;
	}
}

bool vampiro(int base) {
	bool solEncontrada = false;
	string baseStr = to_string(base);

	bool* used = new bool[baseStr.length()];
	for (int c = 0; c < baseStr.length(); c++)
		used[c] = false;

	vuelta_atras(baseStr, base, 0, solEncontrada, used);
	delete[] used;
	return solEncontrada;
}

int main() {
	int casosPrueba = 0; cin >> casosPrueba;

	for (int i = 0; i < casosPrueba; i++) {
		int numero;		cin >> numero;
		if (numCifras(numero) % 2 == 0 && numero > 10) {
			if (vampiro(numero))
				cout << "SI" << endl;
			else
				cout << "NO" << endl;
		}
		else
			cout << "NO" << endl;
	}

	return 0;
}