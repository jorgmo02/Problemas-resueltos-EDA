#pragma once
#include <stdexcept>
#include <queue>
#include <iostream>

using namespace std;

template <typename T>
class colaVIP {
private:
	// número de elementos en la cola
	size_t nelems;

	// prioridades máxima
	size_t prioridades;

	queue<T>* colas;

public:

	// constructor: cola vacía
	colaVIP(size_t a);

	// destructor
	~colaVIP();

	// encolar un elemento
	void push(int p, const T& elem);

	// consultar primer elemento
	T const& front() const;

	// eliminar primer elemento
	void pop();

	// consultar si la cola está vacía
	bool empty() const;

	// consultar el tamaño de la cola
	size_t size() const;

};

template<typename T>
colaVIP<T>::colaVIP(size_t p) : nelems(0), prioridades(p) {
	colas = new queue<T>[p];
}

template<typename T>
colaVIP<T>::~colaVIP() {
	delete[] colas;
}

template<typename T>
void colaVIP<T>::push(int p, const T& elem) { // O(1)
	nelems++;
	colas[p].push(elem);
}

template<typename T>
T const& colaVIP<T>::front() const { // O(p) siendo p el número de prioridades
	if (empty()) {
		throw std::domain_error("Cola vacía");
	}
	else {
		int i = 0;	bool b = true;
		while (b && i < prioridades) {
			if (!colas[i].empty()) {
				b = false;
			}
			else i++;
		}
		return colas[i].front();
	}
}

template<typename T>
void colaVIP<T>::pop() { // O(p)
	if (empty()) {
		throw std::domain_error("Eliminar en cola vacía");
	}
	else {
		int i = 0;	bool b = true;
		while (b && i < prioridades) {
			if (!colas[i].empty()) {
				b = false;
			}
			else i++;
		}
		colas[i].pop();
		nelems--;
	}
}

template<typename T>
bool colaVIP<T>::empty() const { // O(1)
	return size() == 0;
}

template<typename T>
size_t colaVIP<T>::size() const { // O(1)
	return nelems;
}


int main() {
	int casosPrueba; cin >> casosPrueba;
	for (int i = 0; i < casosPrueba; i++) {
		size_t prioridades, cambios, aforo, aforoAct = 0;
		cin >> prioridades >> cambios >> aforo;

		colaVIP<int> fila = colaVIP<int>(prioridades);

		for (int j = 0; j < cambios; j++) {
			char op; cin >> op;
			if (op == '-')
				aforoAct--;
			else {
				int prioridadNuevo, nuevo;	cin >> prioridadNuevo >> nuevo;
				fila.push(prioridadNuevo, nuevo);
			}
			while (aforoAct < aforo && !fila.empty()) {	// o(aforo)
				fila.pop();
				aforoAct++;
			}
		}

		if (fila.empty()) cout << "NADIE";
		else {
			int lim = fila.size();
			for (int k = 0; k < lim; k++) {
				cout << fila.front() << " ";
				fila.pop();
			}
		}
		cout << endl;
	}

	return 0;
}