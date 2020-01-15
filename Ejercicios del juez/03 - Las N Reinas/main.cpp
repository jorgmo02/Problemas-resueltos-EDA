#include <iostream>
using namespace std;

void nReinas(const int N);
void vueltaAtras(const int N, int* sol, int index, bool* filas, bool* diags_asc, bool* diags_desc);
bool es_solucion(const int N, const int* sol, const int index, const bool* filas, const bool* diags_asc, const bool* diags_desc);
bool es_completable(const int N, const int* sol, const int index, const bool* filas, const bool* diags_asc, const bool* diags_desc);
int numSols = 0;

int main() {
	int casosPrueba = 0;	cin >> casosPrueba;
	for (int i = 0; i < casosPrueba; i++) {
		int N;	cin >> N;
		nReinas(N);
		cout << numSols << endl;
		numSols = 0;
	}
	return 0;
}

int diag_asc(int x, int y) { return x + y; } // o(1)
int diag_desc(int x, int y) { return x - y; } // o(1)

// o(1)
bool no_ataca(const int N, const int* sol, const int index, const bool* filas, const bool* diags_asc, const bool* diags_desc) {
	return !filas[sol[index]] && !diags_asc[diag_asc(index, sol[index])] && !diags_desc[diag_desc(index, sol[index]) + N - 1];
}

// o(1)
bool es_solucion(const int N, const int* sol, const int index, const bool* filas, const bool* diags_asc, const bool* diags_desc) {
	return index == N - 1 && no_ataca(N, sol, index, filas, diags_asc, diags_desc);
}

// o(1)
bool es_completable(const int N, const int* sol, const int index, const bool* filas, const bool* diags_asc, const bool* diags_desc) {
	return index < N && no_ataca(N, sol, index, filas, diags_asc, diags_desc);
}

void ponAFalse(bool* arr, int ini, int fin) {
	for (int i = ini; i < fin; i++) arr[i] = false;
}

void vueltaAtras(const int N, int* sol, int index, bool* filas, bool* diags_asc, bool* diags_desc) {
	for (int i = 0; i < N; i++) {
		sol[index] = i;
		if (es_solucion(N, sol, index, filas, diags_asc, diags_desc)) {
			numSols++;
		}
		else if (es_completable(N, sol, index, filas, diags_asc, diags_desc)) {
			filas[i] = true;
			diags_asc[diag_asc(index, i)] = true;
			diags_desc[diag_desc(index, i) + N - 1] = true;
			vueltaAtras(N, sol, index + 1, filas, diags_asc, diags_desc);
			filas[i] = false;
			diags_asc[diag_asc(index, i)] = false;
			diags_desc[diag_desc(index, i) + N - 1] = false;
		}
	}
}


void nReinas(const int N) {
	if (N > 0) {
		int* sol = new int[N];
		bool* diags_asc = new bool[N * 2 - 1];
		bool* diags_desc = new bool[N * 2 - 1];
		bool* filas = new bool[N];
		ponAFalse(diags_asc, 0, N * 2 - 1); ponAFalse(diags_desc, 0, N * 2 - 1); ponAFalse(filas, 0, N);
		vueltaAtras(N, sol, 0, filas, diags_asc, diags_desc);
		delete[] sol;
	}
}