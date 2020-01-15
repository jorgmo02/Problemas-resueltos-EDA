#ifndef BINTREE_EDA_H_
#define BINTREE_EDA_H_

#include <stdexcept> // domain_error
#include <algorithm> // max
#include <memory>    // shared_ptr, make_shared
#include <iomanip>   // setw
#include <iostream>  // endl
#include <vector>
#include <queue>
using namespace std;


template <typename T>
class bintree {
private:
	static const int TREE_INDENTATION = 4;

	/*
	 Nodo que almacena internamente el elemento (de tipo T)
	 y dos 'punteros compartidos', uno al hijo izquierdo y al hijo derecho.
	 */
	class Nodo; // Declaración adelantada para poder definir Link
	using Link = shared_ptr<Nodo>; // Alias de tipo

	class Nodo {
	public:
		Link izq;
		T elem;
		Link der;

		Nodo(const T& elem) : izq(nullptr), elem(elem), der(nullptr) {}
		Nodo(Link izq, const T& elem, Link der) : izq(izq), elem(elem), der(der) {}
	};

	// puntero a la raíz
	Link raiz;

	// constructora privada a partir de un puntero a Nodo
	// Para construir los árboles generados por left() y right()
	bintree(Link r) : raiz(r) {} // O(1)

	// Muestra por 'out' una representación del árbol
	// Adaptado de "ADTs, DataStructures, and Problem Solving with C++", Larry Nyhoff, Person, 2015
	void graph_rec(ostream& out, int indent, Link raiz) const {
		// O(n), donde 'n' es el número de nodos alcanzables desde 'raiz'
		if (raiz != nullptr) {
			graph_rec(out, indent + TREE_INDENTATION, raiz->der);
			out << setw(indent) << " " << raiz->elem << endl;
			graph_rec(out, indent + TREE_INDENTATION, raiz->izq);
		}
	}

public:
	// constructor de árbol vacío
	bintree() : raiz(nullptr) {} // O(1)

	// constructor de árbol hoja
	bintree(const T& elem) : raiz(make_shared<Nodo>(elem)) {} // O(1)

	// constructor de árbol con 2 hijos
	bintree(const bintree<T>& izq, const T& elem, const bintree<T>& der) : // O(1)
		raiz(make_shared<Nodo>(izq.raiz, elem, der.raiz)) {}

	// valor en la raíz (si existe)
	const T& root() const { // O(1)
		if (empty()) {
			throw std::domain_error("No hay raiz en arbol vacio");
		}
		else {
			return raiz->elem;
		}
	}

	// hijo izquierdo (si existe)
	bintree<T> left() const { // O(1)
		if (empty()) {
			throw std::domain_error("No hay hijos en arbol vacio");
		}
		else {
			return bintree(raiz->izq);
		}
	}

	// hijo derecho (si existe)
	bintree<T> right() const { // O(1)
		if (empty()) {
			throw std::domain_error("No hay hijos en arbol vacio");
		}
		else {
			return bintree(raiz->der);
		}
	}

	// saber si el árbol es vacío 
	bool empty() const { // O(1)
		return (raiz == nullptr);
	}

	// altura del árbol
	size_t height() const { // O(n), donde 'n' es el número de nodos en el árbol
		if (empty()) {
			return 0;
		}
		else {
			size_t hl = left().height();
			size_t hr = right().height();
			return max<size_t>(hl, hr) + 1;
		}
	}

	// Muestra por 'out' una representación del árbol
	// Adaptado de "ADTs, DataStructures, and Problem Solving with C++", Larry Nyhoff, Person, 2015
	void graph(ostream& out) const { // O(n), donde 'n' es el número de nodos en el árbol
		out << "==== Tree =====" << endl;
		graph_rec(out, 0, raiz);
		out << "===============" << endl;
	}

	vector<T> preorder() const {	// o(n) siendo n el número de nodos del arbol
		vector<T> v;
		preorder_rec(v, *(this));
		return v;
	}

	void preorder_rec(vector<T>& v, const bintree<T> tree) const {
		if (tree.raiz != nullptr) {
			v.push_back(tree.root());
			preorder_rec(v, tree.left());
			preorder_rec(v, tree.right());
		}
	}

	vector<T> inorder() const {		// o(n) siendo n el número de nodos del arbol
		vector<T> v;
		inorder_rec(v, *(this));
		return v;
	}

	void inorder_rec(vector<T>& v, const bintree<T> tree) const {
		if (tree.raiz != nullptr) {
			inorder_rec(v, tree.left());
			v.push_back(tree.root());
			inorder_rec(v, tree.right());
		}
	}

	vector<T> postorder() const {		// o(n) siendo n el número de nodos del arbol
		vector<T> v;
		postorder_rec(v, *(this));
		return v;
	}

	void postorder_rec(vector<T>& v, const bintree<T> tree) const {
		if (tree.raiz != nullptr) {
			postorder_rec(v, tree.left());
			postorder_rec(v, tree.right());
			v.push_back(tree.root());
		}
	}

	vector<T> levelorder() const {		// o(n) siendo n el número de nodos del arbol
		vector<T> v;
		queue<bintree<T>> cola;
		cola.push(*(this));

		while (!cola.empty()) {
			bintree<T> act = cola.front();
			cola.pop();
			v.push_back(act.root());
			if (act.left().raiz != nullptr)
				cola.push(act.left());
			if (act.right().raiz != nullptr)
				cola.push(act.right());
		}

		return v;
	}
};

#endif BINTREE_EDA_H_

bintree<int> creaArbol() {		// o(n) siendo n el número de nodos del arbol
	int root;
	cin >> root;
	if (root != 0) {
		bintree<int> izq = creaArbol();
		bintree<int> der = creaArbol();
		return bintree<int>(izq, root, der);
	}
	else return bintree<int>();	// árbol vacío
}

int main() {
	int casosPrueba;	cin >> casosPrueba;

	for (int caso = 0; caso < casosPrueba; caso++) {
		vector<int> preorden, inorden, postorden, nivelorden;
		bintree<int> arbol = creaArbol();

		if (!arbol.empty()) {
			preorden = arbol.preorder();
			inorden = arbol.inorder();
			postorden = arbol.postorder();
			nivelorden = arbol.levelorder();
		}

		for (size_t i = 0; i < preorden.size(); i++)
			cout << preorden[i] << " ";
		cout << endl;
		for (size_t i = 0; i < inorden.size(); i++)
			cout << inorden[i] << " ";
		cout << endl;
		for (size_t i = 0; i < postorden.size(); i++)
			cout << postorden[i] << " ";
		cout << endl;
		for (size_t i = 0; i < nivelorden.size(); i++)
			cout << nivelorden[i] << " ";
		cout << endl;
		cout << "==" << endl;
	}
	return 0;
}