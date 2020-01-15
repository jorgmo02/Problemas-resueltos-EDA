#include <stdexcept>
#include <iostream>
using namespace std;


template <class T>
class list{
private:
	class Nodo {
	public:
		T elem;
		Nodo* sig;

		Nodo() : sig(nullptr) {}
		Nodo(const T& elem, Nodo* sig) : elem(elem), sig(sig) {}
	};

	// Nodo fantasma
	Nodo *fantasma;
	Nodo *ultimo;

	// Número de elementos
	size_t nelems;

	// Elimina la lista de nodos
	void libera();

	// Busca el nodo anterior a la posicion ind, si existe
	Nodo* busca_anterior(size_t ind) const;

	// Crea una copia de los nodos que comienzan en 'n'
	// 'n' no puede ser nullptr
	void copia_nodos(Nodo* n, Nodo* &first, Nodo* &last) const;


	// Dadas dos secuencias 'l' y 'r' de nodos ordenados terminadas en nullptr,
	// realiza una mezcla ordenada y devuelve un puntero al primer nodo del resultado
	Nodo* merge(Nodo* l, Nodo* r) const {
		Nodo* ini;
		if (l != nullptr) {
			if (r != nullptr) {
				if (l->elem < r->elem) {
					ini = l;
					l = l->sig;
				}
				else {
					ini = r;
					r = r->sig;
				}
				ini->sig = nullptr;
				Nodo* aux = ini;
				while (l != nullptr && r != nullptr) {
					if (l->elem < r->elem) {
						aux->sig = l;
						l = l->sig;
					}
					else {
						aux->sig = r;
						r = r->sig;
					}
					aux = aux->sig;
					aux->sig = nullptr;
				}
				(l == nullptr) ? aux->sig = r : aux->sig = l;

				return ini;
			}
			else return l;
		}
		else return r;
	}

	// Dada una sencuencia 'n' de nodos terminada en nullptr, la divide en 2 secuencias
	// ('l' y 'r') de longitud aproximadamente la mitad (diferencia de +/-1)
	void split(Nodo* n, Nodo*& l, Nodo*& r) const {
		size_t tam = 0;
		l = n;
		Nodo* aux = n;
		while (aux != nullptr) {
			aux = aux->sig;
			tam++;
		}
		if (tam > 0) {
			int mit = tam / 2;
			aux = n;
			for (int i = 0; i < mit - 1; i++) {
				aux = aux->sig;
			}
			r = aux->sig;
			aux->sig = nullptr;
		}
	}

	// Ordena una secuencia 'n' de nodos terminada en nullptr usando mergesort,
	// devolviendo un puntero al primer nodo del resultado
	Nodo* sort(Nodo* n) const {

		Nodo* l;
		Nodo* r;
		split(n, l, r);

		if (l != nullptr && r != nullptr) {
			l = sort(l);
			r = sort(r);
		}

		return merge(l, r);
	}

public:

    // constructor: lista vacía
    list();

    // constructor por copia
    list(const list<T>& other);

    // destructor
    ~list();

    // Operador de asignación
    list<T>& operator=(const list<T>&);

    // consultar el elemento en la posición ind, si existe (numeradas desde 0)
    T const& at(size_t ind) const;

    // inserta un elemento en la posicion ind
    void insert(size_t ind, T const& elem);

    // elimina el elemento en la posicion ind
    void erase(size_t ind);

    // concatena una lista por la derecha (crea nodos nuevos)
    void concat(const list<T>& other);

    // mueve los nodos de other al final de la lista (y other queda vacía)
    // Si other es la lista actual no realiza ninguna modificación
    void splice(list<T>& other);

    // Comprueba si la lista está vacía
    bool empty() const;

    // Tamaño de la lista
    size_t size() const;

	// Muestra por pantalla para depurar
    void print() {
    	cout << "{" << size() << "} -> ";
        for(unsigned int i = 0; i < size(); ++i) {
        	cout << this->at(i) << " ";
        }
        cout << endl;
    }

	void sort() {
		if (nelems > 1) {
			fantasma->sig = sort(fantasma->sig);
			ultimo = busca_anterior(nelems - 1);
		}
	}
};


template<typename T>
void list<T>::libera() {
	while(fantasma != nullptr) {
		Nodo* borrar = fantasma;
		fantasma = fantasma->sig;
		delete borrar;
	}
	ultimo = nullptr;
}


template<typename T>
typename list<T>::Nodo* list<T>::busca_anterior(size_t ind) const {  // O(ind)
	if (ind > size()) {
		return nullptr;
	} else {
		Nodo* act = fantasma;
		while(ind > 0) {
			act = act->sig;
			ind--;
		}
		return act;
	}
}


template<typename T>
void list<T>::copia_nodos(Nodo* n, Nodo* &first, Nodo* &last) const { // O(m), con m número de nodos desde 'n'
	if (n->sig == nullptr) {
		first = new Nodo(n->elem, nullptr);
		last = first;
	} else {
		Nodo* first_tail;
		Nodo* last_tail;
		copia_nodos(n->sig, first_tail, last_tail);
		first = new Nodo(n->elem, first_tail);
		last = last_tail;
	}
}


template<typename T>
list<T>::list() : fantasma(new Nodo()), ultimo(fantasma), nelems(0) {}


template<typename T>
list<T>::list(const list<T>& other) : nelems(other.nelems) {
	copia_nodos(other.fantasma, fantasma, ultimo);
}


template<typename T>
list<T>::~list() {
	libera();
}


template<typename T>
list<T>& list<T>::operator=(const list<T> &other) {
	if (this != &other) { // evita auto-asignación
		libera();
		nelems = other.nelems;
		copia_nodos(other.fantasma, fantasma, ultimo);
	}
	return *this;
}


template<typename T>
const T& list<T>::at(size_t ind) const { // O(ind)
	if (ind >= size()) {
		throw std::domain_error("Posición inexistente");
	} else {
		Nodo* anterior = busca_anterior(ind); // Seguro que no es nullptr
		return anterior->sig->elem;
	}
}


template<typename T>
void list<T>::insert(size_t ind, T const& elem) { // O(ind)
	Nodo* anterior = busca_anterior(ind);
	if (anterior == nullptr) {
		throw std::domain_error("Posición inválida para insertar");
	} else {
		Nodo* n = new Nodo(elem,anterior->sig);
		anterior->sig = n;
		if (anterior == ultimo )
			ultimo = n;
		nelems++;
	}
}


template<typename T>
void list<T>::erase(size_t ind) { // O(ind)
	if (ind >= size()) {
		throw std::domain_error("Posición inexistente");
	} else {
		Nodo* anterior = busca_anterior(ind); // Seguro que no es nullptr
		Nodo* borrar = anterior->sig;
		anterior->sig = borrar->sig;
		if (borrar == ultimo)
			ultimo = anterior;
		delete borrar;
		nelems--;
	}
}


template<typename T>
void list<T>::concat(const list<T>& other) { // O(other.size())
	if (other.size() > 0) {
		Nodo* head;
		Nodo* last;
		copia_nodos(other.fantasma->sig, head, last); // fantasma->sig != nullptr
		ultimo->sig = head;
		ultimo = last;
		nelems += other.nelems;
	}
}


template<typename T>
void list<T>::splice(list<T>& other) {  // O(1)
	if (this != &other && other.size() > 0) { // Evita splice consigo misma
		ultimo->sig = other.fantasma->sig;
		ultimo = other.ultimo;
		nelems += other.nelems;
		// Vaciamos la lista original y la dejamos en un estado consistente
		other.nelems = 0;
		other.fantasma->sig = nullptr;
		other.ultimo = other.fantasma;
	}
}


template<typename T>
bool list<T>::empty() const {  // O(1)
	return size() == 0;
}


template<typename T>
size_t list<T>::size() const { // O(1)
	return nelems;
}


int main()
{
	int casosPrueba;	cin >> casosPrueba;
	for (int caso = 0; caso < casosPrueba; caso++)
	{
		list<int> lista;
		size_t tam = 0;		cin >> tam;
		for (int i = 0; i < tam; i++) {
			int elem;	cin >> elem;
			lista.insert(i, elem);
		}
		lista.sort();
		for (int i = 0; i < tam; i++)
			cout << lista.at(i) << " ";
		cout << endl;
	}
	return 0;
}