#include <iostream>

using namespace std;

const int tiposMoneda[8]{ 1, 2, 5, 10, 20, 50, 100, 200 };

void vuelta_atras(int (&numMonedas)[8], int objetivo, int restantes, int actSol, int& maxSol, int k);
int calderilla(int(&numMonedas)[8], int objetivo, int monedasRestantes);
bool es_prometedora(int actSol, int monedaMaxima, int maxSol);
bool es_completable(int objetivo, int k, int monedaMaxima);
bool es_solucion(int maxSol, int actSol, int objetivo);

int main()
{
	int casosPrueba = 0;	cin >> casosPrueba;
	for (int i = 0; i < casosPrueba; i++)
	{
		int numMonedas[8];
		int dineroTotal = 0, monedasTotales = 0, cantidadAPagar;
		cin >> cantidadAPagar;
		bool impar = (cantidadAPagar % 2 != 0); //comprobamos si la cantidad a pagar es impar
		bool hayMonedaImpar = false;	//booleano para ver si alguna de las monedas es impar (si no, no se puede pagar una cantidad impar)
										//sólo se usa si la cantidad a pagar es impar

		for (int j = 0; j < 8; j++) {
			int cantidadMoneda; cin >> cantidadMoneda;

			//comprobamos si necesitamos una moneda impar para resolver el problema
			if (impar && !hayMonedaImpar)
				if (cantidadMoneda > 0 && (j == 0 || j == 2))
					hayMonedaImpar = true;		//hay al menos una moneda impar

			monedasTotales += cantidadMoneda;
			dineroTotal += (cantidadMoneda * tiposMoneda[j]);
			numMonedas[j] = cantidadMoneda;
		}

		int sol = -1;
		if (dineroTotal >= cantidadAPagar) { // Cota: imposible pagar si la cantidad es mayor del dinero que tenemos
			if (!impar || (impar && hayMonedaImpar)) { // Cota: imposible llegar a la solución si la cantidad a pagar es impar y sólo tenemos monedas pares
				if (cantidadAPagar == 0) sol = 0;
				else if (cantidadAPagar > 0)
					sol = calderilla(numMonedas, cantidadAPagar, monedasTotales);
			}
		}

		if (sol != -1)
			cout << sol << endl;
		else	//sol == -1 indica que no se ha encontrado ninguna solución (si se hubiera encontrado, se habría sobreescrito)
			cout << "IMPOSIBLE" << endl;
	}
	return 0;
}

// array por referencia sencillamente para ahorrar memoria adicional
int calderilla(int(&numMonedas)[8], int objetivo, int monedasRestantes) {
	int sol = -1;
	vuelta_atras(numMonedas, objetivo, monedasRestantes, 0, sol, 0);
	return sol;
}

void vuelta_atras(int(&numMonedas)[8], int objetivo, int monedaMaxima, int actSol, int& maxSol, int k) {
	int i = 0;
	bool continuar = true;
	while(continuar && i <= numMonedas[k])	// O(n * O(T(k-1))) siendo n el número de monedas del tipo k
	{
		// O(T(k-1))
		if (i * tiposMoneda[k] <= objetivo) // No tiene sentido coger monedas de más si te vas a pasar de tu objetivo
		{
			objetivo -= (i * tiposMoneda[k]);
			actSol += i;
			numMonedas[k] -= i;
			monedaMaxima -= i; // Cota

			if (es_solucion(maxSol, actSol, objetivo))
				maxSol = actSol;

			else if (es_completable(objetivo, k, monedaMaxima) && es_prometedora(actSol, monedaMaxima, maxSol))	// T(k-1) siendo k el número de tipos de monedas sin explorar
				vuelta_atras(numMonedas, objetivo, monedaMaxima, actSol, maxSol, k + 1);

			// volvemos al estado inicial
			objetivo += (i * tiposMoneda[k]);
			actSol -= i;
			numMonedas[k] += i;
			monedaMaxima += i; // Moneda máxima disponible

			i++;
		}
		else continuar = false;
	}
}

bool es_prometedora(int actSol, int monedaMaxima, int maxSol) {
	return (actSol + monedaMaxima > maxSol); // Cota : Solo bajamos de nivel si es posible encontrar una solucion mejor;
}

bool es_completable(int objetivo, int k, int monedaMaxima) {
	return	monedaMaxima > 0 &&					// Cota: Solo bajamos de nivel si quedan monedas
			k < 7 &&							// Solo hacemos llamada recursiva si no estamos en el ultimo nivel
			!(objetivo % 2 != 0 && k == 2);	// Cota : Si el objetivo que nos queda es impar y ya estamos el el nivel 2 (monedas de 5 centimos), sera imposible que
											//lleguemos a un objetivo impar en los siguientes niveles con monedas de 10, 20, 50, 100 0 200 centimos, pues son pares
}

bool es_solucion(int maxSol, int actSol, int objetivo) {
	return objetivo == 0 && actSol > maxSol;
}