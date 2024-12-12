#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int costeRuta(const vector<int>& ruta, const vector<vector<int>>& matrizDistancias) {
	int coste = 0;
	for (int i = 0; i < ruta.size() - 1; i++) {
		coste += matrizDistancias[ruta[i]][ruta[i + 1]];
	}
	coste += matrizDistancias[ruta.back()][ruta.front()];

	return coste;
}


vector<int> algoritmoTabu(const vector<vector<int>> & matrizDistancias,int maxIteraciones, int tamLista) {
    int numCiudades = matrizDistancias.size();

    // Generar solución inicial
    vector<int> rutaActual(numCiudades);
    for (int i = 0; i < numCiudades; i++) {
        rutaActual[i] = i;
    } // Inicializar con {0, 1, ..., numCiudades-1}

    int mejorCoste = costeRuta(rutaActual, matrizDistancias);
    vector<int> mejorRuta = rutaActual;

    // Lista tabú
    vector<vector<int>> listaTabu;

    // Búsqueda tabú
    for (int iter = 0; iter < maxIteraciones; ++iter) {
        vector<vector<int>> vecinos;

        // Generar vecindad (intercambiando dos ciudades)
        for (int i = 1; i < numCiudades - 1; ++i) {
            for (int j = i + 1; j < numCiudades; ++j) {
                vector<int> vecino = rutaActual;
                swap(vecino[i], vecino[j]);
                // Verificar si el vecino no está en la lista tabú
                if (find(listaTabu.begin(), listaTabu.end(), vecino) == listaTabu.end()) {
                    vecinos.push_back(vecino);
                }
            }
        }

        // Seleccionar el mejor vecino
        vector<int> mejorVecino;
        int mejorCosteVecino = INT_MAX;

        for (const auto& vecino : vecinos) {
            int costeVecino = costeRuta(vecino, matrizDistancias);
            if (costeVecino < mejorCosteVecino) {
                mejorVecino = vecino;
                mejorCosteVecino = costeVecino;
            }
        }

        // Si se encontró un vecino mejor, actualizar ruta actual
        if (!mejorVecino.empty()) {
            rutaActual = mejorVecino;

            // Actualizar la mejor solución global
            if (mejorCosteVecino < mejorCoste) {
                mejorRuta = rutaActual;
                mejorCoste = mejorCosteVecino;
            }

            // Actualizar lista tabú
            listaTabu.push_back(rutaActual);
            if (listaTabu.size() > tamLista) {
                listaTabu.erase(listaTabu.begin()); // Quitar el elemento más antiguo
            }
        }

        // Mostrar el progreso
        cout << "Iteración " << iter + 1 << ": Mejor coste = " << mejorCoste << endl;
    }

    return mejorRuta;
    

}




int main() {
    // Matriz de distancias de ejemplo
    vector<vector<int>> matrizDistancias = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int maxIteraciones = 100;    // Número máximo de iteraciones
    int tamañoListaTabu = 5;     // Tamaño de la lista tabú

    // Calcular la mejor ruta
    vector<int> mejorRuta = algoritmoTabu(matrizDistancias, maxIteraciones, tamañoListaTabu);

    return 0;
}
