#include <iostream>

using namespace std;

int main() {								// coste total: o(n)
	int casos;								// o(1)
	int num;								// o(1)
	cin >> casos;							// o(1)
	for (int i = 0; i < casos; i++) {		// número de iteraciones: n siendo n = casos
		cin >> num; //o(1)					// coste de cada iteración: o(1)
		cout << num * 2 << endl; //o(1)		// coste total: o(n)
	}
}