#include <iostream>
using namespace std;

void llena(int* v, int length);
void mergesort(int* v, int ini, int fin);
void merge(int ini, int mid, int fin, int* v);

int main() {
	int numCasos = 0;	cin >> numCasos;
	for (int i = 0; i < numCasos; i++) {
		int cant = 0;	cin >> cant;
		int* v = new int[cant];
		llena(v, cant);
		mergesort(v, 0, cant);
		for (int j = 0; j < cant; j++)
			cout << v[j] << " ";
		cout << endl;
		delete[] v;
	}
	return 0;
}

void merge(int ini, int mid, int fin, int* v) {
	int nl = mid - ini, nr = fin - mid;
	int* arr1 = new int[nl];
	int* arr2 = new int[nr];
	for (int i = 0; i < nl; i++) {
		arr1[i] = v[i + ini];
	}

	for (int i = 0; i < nr; i++) {
		arr2[i] = v[i + mid];
	}

	for (int i = ini, left = 0, right = 0; i < fin; i++) {
		if (right >= nr || (left < nl && arr1[left] < arr2[right])) {
			v[i] = arr1[left];
			left++;
		}
		else {
			v[i] = arr2[right];
			right++;
		}
	}
	delete[] arr1, arr2;
}

void mergesort(int* v, int ini, int fin) {
	if (ini < fin - 1) {
		int mid = (fin + ini) / 2;
		mergesort(v, ini, mid);
		mergesort(v, mid, fin);
		merge(ini, mid, fin, v);
	}
}

void llena(int* v, int length) {
	for (int i = 0; i < length; i++) {
		int elem;	cin >> elem;
		v[i] = elem;
	}
}