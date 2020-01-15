#include <iostream>
#include <vector>

using namespace std;

bool elemento_situado(const vector <int>& v, int ini, int fin);
void llena(vector <int>& v);

int main() {
	int casosPrueba = 0;
	cin >> casosPrueba;
	for (int i = 0; i < casosPrueba; i++) {
		vector<int> v;
		llena(v);
		if (elemento_situado(v, 0, v.size())) //o(log n)
			cout << "SI" << endl;
		else cout << "NO" << endl;
	}
	return 0;
}

bool elemento_situado(const vector <int>& v, int ini, int fin) { // o(log n)
	if (fin - ini > 1) {
		int mit = fin + ini / 2;
		if (v[mit] > mit) {
			return elemento_situado(v, ini, mit);
		}
		else if (v[mit] < mit) {
			return elemento_situado(v, mit, fin);
		}
		else return true;
	}
	else if (fin - ini == 1) return v[ini] == ini;
	else return false;
}

		
/*
siendo n la longitud del vector:

		{	c1 si n <= 1
T(n) =	{
		{	t(n/2) + c2 si n > 1
*/
void llena(vector<int>& v) {
	int cant = 0;
	cin >> cant;
	for (int i = 0; i < cant; i++) {
		int n;
		cin >> n;
		v.push_back(n);
	}
}