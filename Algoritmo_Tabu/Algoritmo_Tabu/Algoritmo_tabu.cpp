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


vector<int> algoritmoTabu(const vector<vector<int>>& matrizDistancias, int maxIteraciones, int tamLista) {
    int numCiudades = matrizDistancias.size();

    // Generar solución inicial, por defecto las ciudades van de 0 a numCiudades -1 
    vector<int> rutaActual(numCiudades);
    for (int i = 0; i < numCiudades; i++) {
        rutaActual[i] = i;
    } 

    int mejorCoste = costeRuta(rutaActual, matrizDistancias);
    vector<int> mejorRuta = rutaActual;
    //Lista tabú
    vector<vector<int>> listaTabu;

    //Búsqueda tabú
    for (int iter = 0; iter < maxIteraciones; ++iter) {
        vector<vector<int>> vecinos;

        //Se genera vecindad intercambiando dos ciudades
        for (int i = 1; i < numCiudades - 1; ++i) {
            for (int j = i + 1; j < numCiudades; ++j) {
                vector<int> vecino = rutaActual;
                swap(vecino[i], vecino[j]);
                //Se verifica si el vecino no está en la lista tabú
                if (find(listaTabu.begin(), listaTabu.end(), vecino) == listaTabu.end()) {
                    vecinos.push_back(vecino);

                }
            }
        }

        //seleccionar el mejor vecino
        vector<int> mejorVecino;
        int mejorCosteVecino = INT_MAX;//el mayor entero posible

        for (const auto& vecino : vecinos) {
            int costeVecino = costeRuta(vecino, matrizDistancias);
            if (costeVecino < mejorCosteVecino) {
                mejorVecino = vecino;
                mejorCosteVecino = costeVecino;
            }
        }
        
        //si se encontró un vecino mejor se actualiza la ruta actual
        if (!mejorVecino.empty()) {
            rutaActual = mejorVecino;

            //actualizar la mejor solución global
            if (mejorCosteVecino < mejorCoste) {
                mejorRuta = rutaActual;
                mejorCoste = mejorCosteVecino;
            }

            // Actualizar lista tabú
            listaTabu.push_back(rutaActual);
            if (listaTabu.size() >= tamLista) {
                listaTabu.erase(listaTabu.begin()); //Quitar el elemento más antiguo
            }
        }


    }
    cout << "Mejor coste = " << mejorCoste << endl;
    return mejorRuta;


}




int main() {
    //Matriz de distancias de ejemplo
    //Las filas representan las ciudades de las que se parte (Origen)
    //Las columnas representan las ciudades destino (Destino)
    //El contenido de la matriz es el coste de llegar de una ciudad origen a una ciudad destino
    
    //EJEMPLO 1
    vector<vector<int>> matrizDistancias = {
        {0, 2, 1, 1},
        {2, 0, 3, 2},
        {1, 3, 0, 4},
        {1, 2, 4, 0}
    }; 

    /*
    //Ejemplo 2
    vector<vector<int>> matrizDistancias = {
        {0, 1, 3, 5},
        {5, 0, 2, 3},
        {1, 1, 0, 1},
        {7, 1, 4, 0}
    };
    */

    int maxIteraciones = 50;    //Num máximo de iteraciones
    int tamañoListaTabu = 5;     //Tamaño de la lista tabú

    // Calcular la mejor ruta
    vector<int> mejorRuta = algoritmoTabu(matrizDistancias, maxIteraciones, tamañoListaTabu);
    cout << "Mejor ruta: ";
    for (int i = 0; i < mejorRuta.size(); i++) {
        cout << mejorRuta[i] + 1 << " -> ";
    }
    cout << mejorRuta[0] + 1;

    return 0;
}
