#include <iostream>
#include <vector>
#include <queue>
using namespace std;


//Aplicamos busqueda por anchura que es mas eficiente que bsuqueda pro profundidad
bool bfs(const vector<vector<int>>& grafoResidual, int origen, int destino, vector<int>& camino) {  
	//Guardamos el numero de nodos en una variable para no escribir la funcion cuando se necesita
	int nNodos = grafoResidual.size();
	//Vector de nodos visitados
	vector<bool> visitado(nNodos,false);	
	//Colas para guardar los nodos a visitar
	queue<int> cola;

	//Insertar el origen en la cola es con el que vamos a empezar
	cola.push(origen);	
	//Marcar origen como visitado
	visitado[origen] = true;
	//El nodo origen no tiene precedente
	camino[origen] = -1;				

	//Mientras la cola no haya terminado seguimos
	while (!cola.empty()) {
		//Sacamos el nodo que esta en la primera posicion
		int u = cola.front();
		//Y lo eliminamos y que lo estamos visitando
		cola.pop();						

		for (int v = 0; v < nNodos; v++) {
			//si no esta visitado el nodo v y hay capacidad
			if (!visitado[v] && grafoResidual[u][v] > 0) {
				//Registrar el camino hacia el nodo v desde el nodo u
				camino[v] = u;
				//Marcamos como visitado
				visitado[v] = true;
				//Lo insertamos en la cola
				cola.push(v);
				
				if (v == destino) {
					//Devolvemos true si conseguimos llegar al destino(sumidero)
					return true;
				}
			}
		}
	}
	//En caso de que no encotramos un camino al destino devolvemos false
	return false;
}

int fordFulkerson(vector<vector<int>>& grafo, int origen, int destino) {
	//Numero de nodos
	int nNodos = grafo.size();
	//GrafoResidual
	vector<vector<int>> grafoResidual = grafo;
	//Guarda el camino encontrado por el BFS
	vector<int> camino(nNodos);
	//Iniciar el flujo maximo por camino
	int maximoFlujo = 0;
	
	while (bfs(grafoResidual,origen,destino,camino)) {
		//Iniciamos a un valor grande para poder enconrtar el maximo flujo que permite el camino
		int maximoFlujoCamino = INT_MAX;
		//Recorremos desde el destino al origen usuando el camino
		for (int v = destino; v != origen; v = camino[v]) {	
			int u = camino[v];
			//calculamos el flujo maximo que permite el camino
			maximoFlujoCamino = min(maximoFlujoCamino,grafoResidual[u][v]);	
		}

		for (int v = destino; v != origen; v = camino[v]) {
			int u = camino[v];
			//Reducimos la capacidad de los nodos que van en direccion al origen
			grafoResidual[u][v] -= maximoFlujoCamino;
			//Aumentamos la capacodad de los ndoos que van en direccion al destino 
			grafoResidual[v][u] += maximoFlujoCamino;		
		}
		//Aumentamos el flujo encontrado en el camino actual
		maximoFlujo += maximoFlujoCamino;					

	}
	//Devolvemos el flujo maximo
	return maximoFlujo;
}

int main() {

	//Matriz de capacidades, siendo grafo[u][v] significa la capacidad que hay entre los nodos u y v.
	/*
	vector<vector<int>> grafo = { 
		{0, 8, 0, 0, 3, 0},	//Nodo 0
		{0, 0, 9, 0, 0, 0},	//Nodo 1
		{0, 0, 0, 0, 7, 2},	//Nodo 2
		{0, 0, 0, 0, 0, 5}, //Nodo 3
		{0, 0, 7, 4, 0, 0}, //Nodo 4
		{0, 0, 0, 0, 0, 0}	//Nodo 5
	};
	*/
	vector<vector<int>> grafo = {
		{0, 10, 10, 0, 0},  // Nodo 0
		{0, 0, 5, 5, 0},    // Nodo 1
		{0, 0, 0, 10, 0},   // Nodo 2
		{0, 0, 0, 0, 10},   // Nodo 3
		{0, 0, 0, 0, 0}		// Nodo 4
	};
	
	//Mostrar la salida
	cout << "Flujo Máximo:" << fordFulkerson(grafo,0,4) << endl;

	return 0;
}