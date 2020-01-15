#include <memory>    // shared_ptr, make_shared
#include <iomanip>   // setw en graphAux
#include <iostream>  // endl
#include <cmath>
#include <utility>
using namespace std;

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
	using pInt = pair<int, int>;

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
	bool search_rec(const T& e, const Link& raiz) const {
		if (raiz == nullptr) {
			return false;
		}
		else if (cmp(e, raiz->elem)) {
			return search_rec(e, raiz->izq);
		}
		else if (cmp(raiz->elem, e)) {
			return search_rec(e, raiz->der);
		}
		else { // e == nodo->elem
			return true;
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
	bool search(const T& e) const {
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

	// calcular distancia entre dos nodos, O(a) donde 'a' es la altura
	int distancia(const T& a, const T& b) const {
		// mira si existen ambos elementos
		if (search(a) && search(b)) {
			Link padre = padreComun(raiz, a, b);
			int distA = distancia_rec(padre, a);
			int distB = distancia_rec(padre, b);
			return distA + distB;
		}
		else return -1;
	}
	
	int distancia_rec(const Link padre, const T& dato) const {
		T* datoPadre = &(padre->elem);
		if (cmp(dato, *(datoPadre)))			// nodo que contiene el dato está a la izquierda del padre
			return 1 + distancia_rec(padre->izq, dato);
		else if (cmp(*(datoPadre), dato))	// nodo que contiene el dato está a la derecha del padre
			return 1 + distancia_rec(padre->der, dato);
		else return 0;						// padre = nodo que contiene el dato
	}

	const Link padreComun(const Link padre, const T& a, const T& b) const {
		T* dato = &(padre->elem);
		if (cmp(a, *(dato)) && cmp(b, *(dato))) {
			return padreComun(padre->izq, a, b);
		}
		else if (cmp(*(dato), a) && cmp(*(dato), b)) {
			return padreComun(padre->der, a, b);
		}
		else return padre;
	}
	
	int altura(const Link arbol) {
		if (arbol == nullptr)
			return 0;

		int altLeft = altura(arbol->izq);
		int altRight = altura(arbol->der);
		return fmax(altLeft, altRight) + 1;
	}

	int diametro_rec(const Link arbol) {
		if (arbol == nullptr)
			return 0;

		int diametroRaizComun = altura(arbol->izq) + altura(arbol->der) + 1;
		int diametroLeft = diametro_rec(arbol->izq);
		int diametroRight = diametro_rec(arbol->der);

		return fmax(diametroRaizComun, fmax(diametroLeft, diametroRight));
	}

	int diametro() {
		return diametro_rec(raiz);
	}

	// devuelve par <diámetro, altura>
	pInt diametro_enN(const Link arbol) const {
		// caso base, arbol hoja
		if (arbol == nullptr)
			return pInt(0, 0);

		pInt left = diametro_enN(arbol->izq);
		pInt rigth = diametro_enN(arbol->der);

		// diametro con raiz comun = altura máxima por la izquierda + altura máxima por la derecha + 1
		int diametroRaizComun = left.second + rigth.second + 1;
		int diametroLeft = left.first;
		int diametroRight = rigth.first;

		int altura = fmax(left.second, rigth.second) + 1;
		return pInt(fmax(diametroRaizComun, fmax(diametroLeft, diametroRight)), altura);
	}

	int diametroEnN() {
		int total = diametro_enN(raiz).first - 1;
		if (total < 0) total = 0;
		return total;
	}

	int diametro_rec_Nico(const Link b, int& diam) const {
		if (b == nullptr)
			return 0;
		else {
			int left = diametro_rec_Nico(b->izq, diam);
			int right = diametro_rec_Nico(b->der, diam);
			(left + right + 1 > diam) ? diam = left + right + 1 : 0;
			if (left > right)
				return left + 1;
			else return right + 1;
		}
	}

	int diametro_Nico() const {
		int diam = 0;
		diametro_rec_Nico(raiz, diam);
		if (diam != 0)
			diam--;
		return diam;
	}
};


int main()
{
	int casosPrueba;	cin >> casosPrueba;
	for (int caso = 0; caso < casosPrueba; caso++) {
		bst<int> arbol = bst<int>();
		int elems = 0;	cin >> elems;
		for (int i = 0; i < elems; i++)
		{
			int elemento;	cin >> elemento;
			arbol.insert(elemento);
		}

		cout << arbol.diametroEnN() << endl;

		int numDistancias = 0;	cin >> numDistancias;
		for (int i = 0; i < numDistancias; i++) {
			int elem1, elem2; cin >> elem1 >> elem2;
			cout << arbol.distancia(elem1, elem2) << endl;	// si da timed-out, quitar las comprobaciones de que los elementos existen en el método distancia
		}
		
		cout << endl;
	}

	return 0;
}