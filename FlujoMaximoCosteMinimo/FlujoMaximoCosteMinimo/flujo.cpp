#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

// struct que representa una arista del grafo
struct tArista {
    int hasta;      // vertice destino de la arista
    int capacidad;  // capacidad de la arista
    int coste;      // coste de la arista 
    int flujo = 0;  // flujo actual
    int reversa;    // indice de la arista reversa

    tArista(int hasta, int capacidad, int coste, int reversa) :
        hasta(hasta), capacidad(capacidad), coste(coste), flujo(0), reversa(reversa) {}
};

// clase que resuelve el problema
class BusackerGowen {
private:
    const int INF = numeric_limits<int>::max();
    int n;                                      // numero de vertices
    vector<vector<tArista>> lista_adyacencia;  // matriz de adyacencia

    // funcion que encuentra el camino mas corto implementando el algoritmo de Bellman
    bool caminoMinimoBellman(int origen, int destino, vector<int>& dist, vector<int>& ult, vector<int>& arista) {

        // se limpian los vectores utilizados y se les asigna el valor inicial
        dist.assign(n, INF);    // distancia del origen a cada vertice del grafo
        ult.assign(n, -1);      // vertice predecesor del vertice en el camino mas corto
        arista.assign(n, -1);   // arista que se utiliza para unir el vertice con su vertice predecesor

        dist[origen] = 0;       // la distancia del origen al propio origen es 0

        int i = 0;
        bool updated = true;

        // se repite V - 1 veces
        while(i < (n - 1) && updated){
            updated = false;

            // se recorren todos los vertices del grafo
            for (int v = 0; v < n; v++) {

                // si el vertice aun no ha sido procesado
                if (dist[v] != INF) {

                    // se procesan todas sus aristas adyacentes
                    for (int j = 0; j < lista_adyacencia[v].size(); j++) {
                        tArista& e = lista_adyacencia[v][j];

                        // relajando las aristas
                        if (e.capacidad > e.flujo && dist[e.hasta] > dist[v] + e.coste) {
                            dist[e.hasta] = dist[v] + e.coste;
                            ult[e.hasta] = v;
                            arista[e.hasta] = j;
                            updated = true;
                        }
                    }
                }
            }
            i++;
        }
        return dist[destino] != INF;
    }

public:

    // constructora
    BusackerGowen(int n) : n(n) {
        lista_adyacencia.resize(n);
    }

    // anyade una arista al grafo
    void ponArista(int desde, int hasta, int capacidad, int coste) {
        lista_adyacencia[desde].push_back(tArista(hasta, capacidad, coste, lista_adyacencia[hasta].size()));
        lista_adyacencia[hasta].push_back(tArista(desde, 0, -coste, lista_adyacencia[desde].size() - 1));
    }

    // encuentra el flujo maximo a coste minimo, resolviendo el problema
    pair<int, int> flujoCosteMinimo(int origen, int destino) {
        int flujo = 0;
        int coste = 0;
        vector<int> dist, ult, arista;

        // mientras siga habiendo un camino del origen al destino
        while (caminoMinimoBellman(origen, destino, dist, ult, arista)) {

            // se encuentra el flujo de coste minimo de este
            int curr = destino;
            int minimo_flujo = INT_MAX;

            // se recorre el camino de adelante hacia atras, hasta llegar al origen
            while (curr != origen) {
                int prev = ult[curr];
                int e = arista[curr];

                // se encuentra el vertice con menor capacidad actual
                minimo_flujo = min(minimo_flujo, lista_adyacencia[prev][e].capacidad - lista_adyacencia[prev][e].flujo);

                curr = prev;
            }

            // tras ello, se actualizan el coste y flujo
            curr = destino;             // se vuelve a recorrer el camino en orden inverso
            while (curr != origen) {
                int prev = ult[curr];
                int e = arista[curr];

                // se actualizan los valores
                lista_adyacencia[prev][e].flujo += minimo_flujo;
                lista_adyacencia[curr][lista_adyacencia[prev][e].reversa].flujo -= minimo_flujo;
                coste += minimo_flujo * lista_adyacencia[prev][e].coste;

                curr = prev;
            }

            // se actualiza el flujo
            flujo += minimo_flujo;
        }

        // se devuelven las soluciones
        return { flujo, coste };
    }
};

int main() {
    // se genera el grafo, con n nodos
    BusackerGowen g(4);

    // se construye el grafo anyadiendo las aristas deseadas (desde, hasta, capacidad, coste)
    
    /*g.ponArista(0, 1, 12, 7);
    g.ponArista(0, 2, 21, 4);
    g.ponArista(0, 3, 17, 9);
    g.ponArista(0, 4, 30, 7);
    g.ponArista(1, 5, 19, 6);
    g.ponArista(2, 1, 16, 1);
    g.ponArista(2, 3, 18, 3);
    g.ponArista(3, 5, 18, 3);
    g.ponArista(4, 3, 15, 1);
    g.ponArista(4, 5, 16, 4);*/

    
    g.ponArista(0, 1, 10, 2);
    g.ponArista(0, 2, 5, 6);
    g.ponArista(1, 2, 15, 2);
    g.ponArista(1, 3, 10, 1);
    g.ponArista(2, 3, 10, 3);

    // se calcula el flujo de costo minimo desde el primer nodo (origen), hasta el ultimo (destino)
    auto [flujo_maximo, coste_minimo] = g.flujoCosteMinimo(0, 3);

    cout << "Flujo maximo: " << flujo_maximo << endl;
    cout << "Coste minimo: " << coste_minimo << endl;

    return 0;
}