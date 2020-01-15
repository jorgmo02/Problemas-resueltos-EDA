#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>

using namespace std;

template<typename T, typename Equal = equal_to <T>>
tuple<int, T> divideYVenceras(const vector<T>& v, int ini, int fin) {
	if (fin - ini > 1) {
		Equal eq;

		int mit = (ini + fin) / 2;
		tuple<int, T> p1 = divideYVenceras(v, ini, mit);
		tuple<int, T> p2 = divideYVenceras(v, mit, fin);

		if (eq(get<1>(p1), get<1>(p2))) {	// p2 comparte mayoritario con p1
			get<0>(p1) += get<0>(p2);
			return p1;
		}
		else if (get<0>(p2) > get<0>(p1)) {	// la frecuencia del mayoritario de p2 es mayor que la de p1
			for (int k = ini; k < mit; k++) {
				if (eq(get<1>(p2), v[k]))
					get<0>(p2)++;
			}
			return p2;
		}
		else {								// la frecuencia del mayoritario de p1 es mayor que la de p2
			for (int k = mit; k < fin; k++) {
				if (eq(get<1>(p1), v[k]))
					get<0>(p1)++;
			}
			return p1;
		}
	}
	else return tuple<int, T>(1, v[ini]);
}

template <typename T, typename Equal = equal_to <T>>
pair <bool, T> mayoria(const vector <T >& v) {
	tuple<int, T> res = divideYVenceras(v, 0, v.size());
	bool b = get<0>(res) > v.size() / 2;
	return pair<bool, T>(b, get<1>(res));
}

int main()
{
	int caso;	cin >> caso;

	while (caso != -1)
	{
		if (caso > 0) {

			vector<int> v;
			for (int i = 0; i < caso; i++) {
				int elem;	cin >> elem;
				v.push_back(elem);
			}

			pair<bool, int> p = mayoria(v);

			if (p.first)
				cout << p.second << endl;
			else cout << "NADA" << endl;
		}
		else cout << "NADA" << endl;

		cin >> caso;
	}
	return 0;
}