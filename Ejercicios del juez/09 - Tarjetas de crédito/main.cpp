#include <memory>    // shared_ptr, make_shared
#include <iomanip>   // setw en graphAux
#include <iostream>  // endl
#include <utility>	 // pair< x, y >
#include <stdexcept> // domain_error
#include <vector>

using namespace std;

template <typename Key, typename Value>
class Menor {
public:
	bool operator()(const pair<Key, Value>& lhs, const pair<Key, Value>& rhs) const {
		return lhs.first < rhs.first;
	}
};

/*
 T: tipo de los elementos almacenados en el bst
 Compare: clase que implementa operator() aceptando 2 referencias constantes a valores de tipo T, y devuelve
		  si el primer valor es "menor" que el segundo. Debe ser un 'strict weak ordering':
		  (1) irreflexivo, (2) asimétrico, (3) transitivo y con (4) transitividad de la incomparabilidad.
	(Ver más detalles en https://en.wikipedia.org/wiki/Weak_ordering#Strict_weak_orderings)
*/
template <typename T, typename Compare = less<T>>
class bst {
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

	// objeto comparador
	Compare cmp;


	// busqueda de nodo, O(a) donde 'a' es la altura del BST comenzando en raiz
	pair<bool, T> search_rec(const T& e, const Link& raiz) const {
		if (raiz == nullptr) {
			return pair<bool, T>(false, T());
		}
		else if (cmp(e, raiz->elem)) {
			return search_rec(e, raiz->izq);
		}
		else if (cmp(raiz->elem, e)) {
			return search_rec(e, raiz->der);
		}
		else { // e == nodo->elem
			return pair<bool, T>(true, raiz->elem);
		}
	}


	// cálculo recursivo del tamaño, O(a) donde 'a' es la altura del BST comenzando en nodo
	size_t size_rec(const Link& nodo) const {
		if (nodo == nullptr) {
			return 0;
		}
		else {
			return size_rec(nodo->izq) + size_rec(nodo->der) + 1;
		}
	}


	// inserción recursiva de elemento, O(a) donde 'a' es la altura del BST comenzando en raiz
	void insert_rec(Link& raiz, const T& e) {
		if (raiz == nullptr) {
			raiz = make_shared<Nodo>(e);
		}
		else if (cmp(e, raiz->elem)) {
			insert_rec(raiz->izq, e);
		}
		else if (cmp(raiz->elem, e)) {
			insert_rec(raiz->der, e);
		}
		else {
			// Si e == raiz-elem el elemento ya esta, puedo lanzar una excepcion 
			// o no hacer nada
		}
	}


	// Dada la raiz de un árbol NO VACIO (es decir, raiz != nullptr), desconecta 
	// el nodo mínimo y lo devuelve por separado
	// O(a) donde 'a' es la altura del BST comenzando en raiz
	Link remove_min(Link& raiz) {
		if (raiz->izq == nullptr) { // El nodo raiz contiene el mínimo
			Link aux = raiz;
			raiz = raiz->der;
			aux->der = nullptr; // lo desconectamos del árbol, por limpieza
			return aux; // El nodo aux no se libera, aun tiene un dueño
		}
		else {
			return remove_min(raiz->izq);
		}
	}


	// Elimina el elemento 'e' del BST si existe
	// O(a) donde 'a' es la altura del BST comenzando en raiz
	void remove_rec(Link& raiz, const T& e) {
		if (raiz == nullptr) // Imposible eliminar en árbol vacío
			return;

		if (cmp(e, raiz->elem)) {
			remove_rec(raiz->izq, e);
		}
		else if (cmp(raiz->elem, e)) {
			remove_rec(raiz->der, e);
		}
		else if (raiz->der == nullptr) { // e == raiz->elem
			raiz = raiz->izq; // se libera la memoria del nodo
		}
		else { // e == raiz->elem && raiz->der != nullptr
			Link nueva_raiz = remove_min(raiz->der);
			// Conecta los hijos de la raíz antigua a la nueva raiz
			nueva_raiz->izq = raiz->izq;
			nueva_raiz->der = raiz->der;
			raiz = nueva_raiz; // se libera el nodo con el valor 'e' que queríamos eliminar
		}
	}


	// Muestra por 'out' una representación del árbol
	// Adaptado de "ADTs, DataStructures, and Problem Solving with C++", Larry Nyhoff, Person, 2015
	// O(n), donde 'n' es el numero de elementos del BST comenzando en raiz
	void graph_rec(ostream& out, int indent, Link raiz) const {
		if (raiz != nullptr) {
			graph_rec(out, indent + TREE_INDENTATION, raiz->der);
			out << setw(indent) << " " << raiz->elem << endl;
			graph_rec(out, indent + TREE_INDENTATION, raiz->izq);
		}
	}


	// Realiza una copia de todos los nodos que cuelgan de 'raiz'
	// O(n), donde 'n' es el numero de elementos del BST comenzando en raiz
	Link copia(Link raiz) {
		if (raiz == nullptr) {
			return nullptr;
		}
		else {
			Link ni = copia(raiz->izq);
			Link nd = copia(raiz->der);
			return make_shared<Nodo>(ni, raiz->elem, nd);
		}
	}

public:
	// constructor de bst vacío
	bst() : raiz(nullptr) {}


	// constructor por copia
	// O(n), donde 'n' es el numero de elementos del BST other
	bst(const bst<T, Compare>& other) {
		raiz = copia(other.raiz);
	}

	// Asignacion
	// O(n), donde 'n' es el numero de elementos del BST other
	bst<T, Compare>& operator=(const bst<T, Compare>& other) {
		if (this != &other) {
			raiz.reset();
			raiz = copia(other.raiz);
		}
		return *this;
	}

	// saber si el bst es vacío, O(1)
	bool empty() const {
		return (raiz == nullptr);
	}


	// tamaño del bst, O(n) donde 'n' es el número de nodos en el BST
	size_t size() const {
		return size_rec(raiz);
	}

	// buscar un elemento, O(a) donde 'a' es la altura del arbol
	pair<bool, T> search(const T& e) const{
		return search_rec(e, raiz);
	}


	// insertar un elemento, O(a) donde 'a' es la altura del arbol
	void insert(const T& e) {
		insert_rec(raiz, e);
	}


	// borrar un elemento, O(a) donde 'a' es la altura del arbol
	void remove(const T& e) {
		remove_rec(raiz, e);
	}


	// Muestra por 'out' una representación del árbol
	// Adaptado de "ADTs, DataStructures, and Problem Solving with C++", Larry Nyhoff, Person, 2015
	// O(n) donde 'n' es el número de elementos del arbol
	void graph(ostream& out) const {
		out << "==== Tree =====" << endl;
		graph_rec(out, 0, raiz);
		out << "===============" << endl;
	}
};

/*
 Key: tipo de las claves
 Value: tipo de los valores
 Compare: clase que implementa operator() aceptando dos elementos de tipo K y calcula
	 un valor booleano. Representa la noción de menor entre claves
*/
template <typename Key, typename Value, typename Compare = less<Key>>
class map {

using par = pair<Key, Value>;

private:

	bst<par, Compare> tree;

public:

	Value get(const Key& key) const {
		return tree.search(par(key, Value())).second.second;
	}

	bool contains(const Key& key) const {
		return tree.search(par(key, Value())).first;
	}

	void insert(const Key& key, const Value& value) {
		if (contains(key))
			erase(key);
		tree.insert(par(key, value));
	}

	void erase(const Key& key) {
		tree.remove(par(key, Value()));
	}
};

int main()
{
	cout << fixed << setprecision(2);

	int casosPrueba = 0;	cin >> casosPrueba;

	for (int caso = 0; caso < casosPrueba; caso++) {

		int operaciones = 0;	cin >> operaciones;
		map<size_t, float, Menor<size_t, float>> tarjetas;

		for (int i = 0; i < operaciones; i++) {
			size_t TC;
			float dinero;
			cin >> TC >> dinero;

			tarjetas.insert(TC, dinero + tarjetas.get(TC));
		}

		cin >> operaciones;

		for (int op = 0; op < operaciones; op++) {
			size_t TC;
			cin >> TC;
			cout << TC << " " << tarjetas.get(TC) << endl;
		}

		cout << endl;
	}

	return 0;
}

/*
2
3
405062 56.34
487379 12.20
405062 -10.34
3
405062
487379
547329
2
487379 12.20
487379 -12.21
1
487379
*/