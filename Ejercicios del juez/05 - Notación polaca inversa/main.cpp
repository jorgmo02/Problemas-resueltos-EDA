#include <iostream>
#include <stack>

using namespace std;
void opera(stack<int>& pila, char operador);

int main()
{
	int casosPrueba = 0; cin >> casosPrueba;
	
	for (int i = 0; i < casosPrueba; i++) {
		int caracteres = 0;		cin >> caracteres;
		stack<int> pila;
		char c = getchar();
		for (int j = 0; j < caracteres; j++) {
			c = getchar();
			if (c >= '0' && c <= '9') {
				pila.push(c - '0');
			}

			else opera(pila, c);
		}

		if(!pila.empty()) cout << pila.top() << endl;
	}

	return 0;
}

void opera(stack<int>& pila, char operador) {
	int prevTop = pila.top();
	pila.pop();
	int result = prevTop;
	if (operador == '-')
		result = pila.top() - prevTop;
	else if (operador == '+')
		result = pila.top() + prevTop;
	pila.pop();
	pila.push(result);
}