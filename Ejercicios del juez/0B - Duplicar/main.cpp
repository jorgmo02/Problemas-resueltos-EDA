#include <iostream>

using namespace std;

int main() {
	int duplicado = 0;
	cin >> duplicado;
	while (duplicado != 0) {
		cout << duplicado * 2 << endl;
		cin >> duplicado;
	}
	return 0;
}