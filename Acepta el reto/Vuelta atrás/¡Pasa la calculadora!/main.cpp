#include <iostream>

using namespace std;

const int MAT[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };

bool intento_vueltaAtrasMasEficiente(int act, int prevJugada);
int min(int jugada);
int max(int jugada, int act);

// DEVUELVE EL NÚMERO MÍNIMO QUE SE PUEDE ESCOGER
int min(int jugada) {
	int col = (jugada - 1) % 3;
	if (col == 0) col++;
	else col = 0;

	return MAT[0][col];
}

// DEVUELVE EL NÚMERO MÁXIMO QUE SE PUEDE ESCOGER SIN PERDER
int max(int jugada, int act) {

	int maxNum = 31 - 1 - act;
	int filJugada = (jugada - 1) / 3, colJugada = (jugada - 1) % 3;

	if (maxNum > 0) {	// else ha perdido automáticamente

		// escoge el inmediatamente inferior
		if (maxNum == jugada) {
			if (colJugada > 0)
				colJugada--;
			else if (filJugada > 0) {
				filJugada--;
			}
			else return 31;	// else ha perdido
		}

		// escoge el máximo posible de jugada
		else if (maxNum > jugada) {
			if (filJugada < 2) {
				filJugada = 2;
			}
			else {
				if (colJugada == 2) {
					return 8;
				}
				return 9;
			}
		}
		else {
			colJugada = (jugada - (maxNum)) % 3;
			filJugada = (maxNum - 1) / 3;
			if (MAT[filJugada][colJugada] > maxNum) filJugada--;
		}
		return MAT[filJugada][colJugada];
	}
	else return 31; // pierde automáticamente
}

bool noPierdeSigJugada(int base, int jug) {

	int enemyJug = max(jug, base);
	return enemyJug + base >= 31 || enemyJug + base + min(enemyJug) < 31;
}

bool intento_vueltaAtrasMasEficiente(int act, int prevJugada) {

	int newJugada = 1;
	bool ganaJug = false;

	// bucle prueba todas las posibilidades mientras el jugador gane
	while (!ganaJug && newJugada <= 9) {

		// jugada válida
		if (newJugada != prevJugada && ((newJugada - 1) / 3 == (prevJugada - 1) / 3 || (newJugada - 1) % 3 == (prevJugada - 1) % 3)) {
			
			// si no pierde con esta...
			if (newJugada + act < 31) {

				// calcula cómo irá la jugada siguiente teniendo en cuenta que el otro jugador
				// escoge la mejor posible sin perder
				if (noPierdeSigJugada(newJugada + act, newJugada)) {
					ganaJug = !intento_vueltaAtrasMasEficiente(newJugada + act, newJugada);
				}
				else ganaJug = true;
			}
		}

		newJugada++;
	}
	return ganaJug;
}

bool gana(int act, int prevJugada) {

	// caso base, el jugador pierde
	if (act > 30)
		return false;

	// bucle prueba todas las posibilidades de jugada
	for (int newJugada = 1; newJugada <= 9; newJugada++) {
		// solo entra si es una tecla válida (4 veces desde i = 1 hasta i = 9)
		if (newJugada != prevJugada &&	// jugada distinta a la anterior
			((newJugada - 1) / 3 == (prevJugada - 1) / 3 || (newJugada - 1) % 3 == (prevJugada - 1) % 3)) // misma fila o columna que la jugada anterior
//			if(!noPierdeSigJugada(act + newJugada, newJugada))
			if (gana(act + newJugada, newJugada))
				return false;	//maravillosa ruptura de flujo (es broma, no lo hagas)
	}
	return true;
}

int main() {

	int casosPrueba = 0;	cin >> casosPrueba;
	for (int caso = 0; caso < casosPrueba; caso++)
	{
		int act, jugada;	cin >> act >> jugada;
		if (!gana(act, jugada))
			cout << "GANA" << endl;
		else cout << "PIERDE" << endl;
	}

	return 0;
}