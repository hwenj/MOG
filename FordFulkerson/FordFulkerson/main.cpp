#include <iostream>
#include <vector>
#include <queue>
using namespace std;


// Se aplica busqueda por anchura pues es mas eficiente que la busqueda en profundidad
bool bfs(const vector<vector<int>>& grafoResidual, int origen, int destino, vector<int>& camino) {  
	// Numero de nodos
	int nNodos = grafoResidual.size();

	// Vector de nodos visitados
	vector<bool> visitado(nNodos, false);	

	// Cola para guardar los nodos a visitar
	queue<int> cola;

	// Se inserta el origen en la cola para empezar en el
	cola.push(origen);	

	// Se marca el origen como visitado
	visitado[origen] = true;

	// El nodo origen no tiene precedente
	camino[origen] = -1;				

	// Mientras no queden nodos en la cola
	while (!cola.empty()) {

		// Se saca el nodo que esta en la primera posicion
		int u = cola.front();

		// Y se elimina porque esta siendo visitado
		cola.pop();						

		// Se recorren los nodos adyacentes a este
		for (int v = 0; v < nNodos; v++) {

			// Y, si no estan visitado y tienen capacidad
			if (!visitado[v] && grafoResidual[u][v] > 0) {

				// Se registra el camino hacia el nodo v desde el nodo u
				camino[v] = u;

				// Se marca como visitado
				visitado[v] = true;

				// Se inserta en la cola
				cola.push(v);
				
				if (v == destino) {
					// Se devuelve true si se llega al destino(sumidero)
					return true;
				}
			}
		}
	}

	// En caso de no encontrar camino, se devuelve false
	return false;
}

int fordFulkerson(vector<vector<int>>& grafo, int origen, int destino) {
	// Numero de nodos
	int nNodos = grafo.size();

	// Grafo Residual, es decir, el grafo con los flujos actuales tras elegir un camino
	vector<vector<int>> grafoResidual = grafo;

	// Camino de la BFS
	vector<int> camino(nNodos);

	// Flujo maximo de la red, solucion del problema
	int maximoFlujo = 0;
	
	// Mientras siga habiendo un camino
	while (bfs(grafoResidual,origen,destino,camino)) {

		// Se inicializa ell maximo flujo del camino a un valor grande
		int maximoFlujoCamino = INT_MAX;

		// Se recorren todos los nodos del camino
		for (int v = destino; v != origen; v = camino[v]) {	
			int u = camino[v];

			// Y se calcula el flujo maximo de la red, siendo este el flujo minimo residual del camino
			maximoFlujoCamino = min(maximoFlujoCamino,grafoResidual[u][v]);	
		}

		// Tras calcular el flujo, se actualizan las capacidades actuales de cada nodo
		for (int v = destino; v != origen; v = camino[v]) {
			int u = camino[v];

			// Se reduce la capacidad de los nodos que van en direccion al origen
			grafoResidual[u][v] -= maximoFlujoCamino;

			// Se aumenta la capacidad de los nodos que van en direccion al destino 
			grafoResidual[v][u] += maximoFlujoCamino;		
		}

		// Se aumenta el flujo maximo con el encontrado en el camino actual
		maximoFlujo += maximoFlujoCamino;					

	}

	// Se devuelve el flujo maximo
	return maximoFlujo;
}

int main() {

	// Matriz de capacidades, siendo grafo[u][v] significa la capacidad que hay entre los nodos u y v.
	
	vector<vector<int>> grafo = { 
		{0, 8, 0, 0, 3, 0},	//Nodo 0
		{0, 0, 9, 0, 0, 0},	//Nodo 1
		{0, 0, 0, 0, 7, 2},	//Nodo 2
		{0, 0, 0, 0, 0, 5}, //Nodo 3
		{0, 0, 7, 4, 0, 0}, //Nodo 4
		{0, 0, 0, 0, 0, 0}	//Nodo 5
	};
	
	/*
	vector<vector<int>> grafo = {
		{0, 10, 10, 0, 0},  // Nodo 0
		{0, 0, 5, 5, 0},    // Nodo 1
		{0, 0, 0, 10, 0},   // Nodo 2
		{0, 0, 0, 0, 10},   // Nodo 3
		{0, 0, 0, 0, 0}		// Nodo 4
	};*/
	
	// Se muestra la solucion, 0 siempre es el nodo origen y el nodo destino es el numero de nodos del grafo.
	cout << "Flujo Maximo: " << fordFulkerson(grafo,0,5) << endl;

	return 0;
}