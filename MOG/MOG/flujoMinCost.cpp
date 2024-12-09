#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <fstream>

using namespace std;

// T es el tipo de las prioridades
// Comparator dice cuándo un valor de tipo T es más prioritario que otro
template <typename T = int, typename Comparator = std::less<T>>
class IndexPQ {
public:
    // registro para las parejas < elem, prioridad >
    struct Par {
        int elem;
        T prioridad;
    };

private:
    // vector que contiene los datos (pares < elem, prio >)
    std::vector<Par> array;     // primer elemento en la posición 1

    // vector que contiene las posiciones en array de los elementos
    std::vector<int> posiciones;   // un 0 indica que el elemento no está

    /* Objeto función que sabe comparar prioridades.
     antes(a,b) es cierto si a es más prioritario que b */
    Comparator antes;

public:
    IndexPQ(int N, Comparator c = Comparator()) : array(1), posiciones(N, 0), antes(c) {}

    IndexPQ(IndexPQ<T, Comparator> const&) = default;

    IndexPQ<T, Comparator>& operator=(IndexPQ<T, Comparator> const&) = default;

    ~IndexPQ() = default;

    // e debe ser uno de los posibles elementos
    void push(int e, T const& p) {
        if (posiciones.at(e) != 0)
            throw std::invalid_argument("No se pueden insertar elementos repetidos.");
        else {
            array.push_back({ e, p });
            posiciones[e] = size();
            flotar(size());
        }
    }

    void update(int e, T const& p) {
        int i = posiciones.at(e);
        if (i == 0) // el elemento e se inserta por primera vez
            push(e, p);
        else {
            array[i].prioridad = p;
            if (i != 1 && antes(array[i].prioridad, array[i / 2].prioridad))
                flotar(i);
            else // puede hacer falta hundir a e
                hundir(i);
        }
    }

    int size() const {
        return int(array.size()) - 1;
    }

    bool empty() const {
        return size() == 0;
    }

    Par const& top() const {
        if (size() == 0)
            throw std::domain_error("No se puede consultar el primero de una cola vacia");
        else return array[1];
    }

    void pop() {
        if (size() == 0) throw std::domain_error("No se puede eliminar el primero de una cola vacía.");
        else {
            posiciones[array[1].elem] = 0; // para indicar que no está
            if (size() > 1) {
                array[1] = std::move(array.back());
                posiciones[array[1].elem] = 1;
                array.pop_back();
                hundir(1);
            }
            else
                array.pop_back();
        }
    }

    T const& priority(int e) const {
        int i = posiciones.at(e);
        if (i == 0)
            throw std::domain_error("No se puede consultar la prioridad de un elemento que no ha sido insertado aún.");
        return array[i].prioridad;
    }

private:

    void flotar(int i) {
        Par parmov = std::move(array[i]);
        int hueco = i;
        while (hueco != 1 && antes(parmov.prioridad, array[hueco / 2].prioridad)) {
            array[hueco] = std::move(array[hueco / 2]); posiciones[array[hueco].elem] = hueco;
            hueco /= 2;
        }
        array[hueco] = std::move(parmov); posiciones[array[hueco].elem] = hueco;
    }

    void hundir(int i) {
        Par parmov = std::move(array[i]);
        int hueco = i;
        int hijo = 2 * hueco; // hijo izquierdo, si existe
        while (hijo <= size()) {
            // cambiar al hijo derecho de i si existe y va antes que el izquierdo
            if (hijo < size() && antes(array[hijo + 1].prioridad, array[hijo].prioridad))
                ++hijo;
            // flotar el hijo si va antes que el elemento hundiéndose
            if (antes(array[hijo].prioridad, parmov.prioridad)) {
                array[hueco] = std::move(array[hijo]); posiciones[array[hueco].elem] = hueco;
                hueco = hijo; hijo = 2 * hueco;
            }
            else break;
        }
        array[hueco] = std::move(parmov); posiciones[array[hueco].elem] = hueco;
    }

};

template <typename Valor>
class AristaDirigida {
public:
    AristaDirigida() : pimpl(nullptr) {}
    AristaDirigida(int v, int w, Valor capacidad, Valor coste) : pimpl(std::make_shared<AristaDirigida_impl>(v, w, capacidad, coste)) {}
    int desde() const { return pimpl->v; }
    int hasta() const { return pimpl->w; }
    Valor capacidad() const { return pimpl->capacidad; }
    Valor coste() const { return pimpl->coste; }
    Valor flujo() const { return pimpl->flujo; }

    void addFlujo(int flujo) {
        pimpl->flujo += flujo;
    }

    void print(std::ostream& o = std::cout) const {
        o << "(" << pimpl->v << ", " << pimpl->w << ", " << pimpl->valor << ")";
    }

    bool operator<(AristaDirigida<Valor> const& b) const {
        return coste() < b.coste();
    }
    bool operator>(AristaDirigida<Valor> const& b) const {
        return b.coste() < coste();
    }
private:
    struct AristaDirigida_impl {
        int v, w;
        Valor capacidad;
        Valor coste;
        Valor flujo;
        AristaDirigida_impl(int v, int w, Valor capacidad, Valor coste)
            : v(v), w(w), capacidad(capacidad), coste(coste), flujo(0) {}
    };
    std::shared_ptr<AristaDirigida_impl> pimpl;
};

template <typename Valor>
inline std::ostream& operator<<(std::ostream& o, AristaDirigida<Valor> const& ar) {
    ar.print(o);
    return o;
}



template <typename Valor>
using AdysDirVal = std::vector<AristaDirigida<Valor>>;  // lista de adyacentes a un vértice

template <typename Valor>
class DigrafoValorado {
public:

    /**
     * Crea un grafo con V vértices.
     */
    DigrafoValorado(int v) : _V(v), _A(0), _ady(_V) {}

    /**
     * Crea un grafo dirigido y valorado a partir de los datos en el flujo de entrada (si puede).
     * primer es el índice del primer vértice del grafo en el entrada.
     */
    DigrafoValorado(std::istream& flujo, int primer = 0) : _A(0) {
        flujo >> _V;
        if (!flujo) return;
        _ady.resize(_V);
        int E, v, w;
        Valor capacidad;
        Valor coste;
        flujo >> E;
        while (E--) {
            flujo >> v >> w >> capacidad >> coste;
            ponArista({ v - primer, w - primer, capacidad, coste });
        }
    }

    /**
     * Devuelve el número de vértices del grafo.
     */
    int V() const { return _V; }

    /**
     * Devuelve el número de aristas del grafo.
     */
    int A() const { return _A; }

    /**
     * Añade la arista dirigida v-w al grafo.
     * @throws invalid_argument si algún vértice no existe
     */
    void ponArista(AristaDirigida<Valor> arista) {
        int v = arista.desde(), w = arista.hasta();
        if (v < 0 || v >= _V || w < 0 || w >= _V)
            throw std::invalid_argument("Vertice inexistente");
        
       auto& listaAdy = _ady[v];
    auto it = std::lower_bound(listaAdy.begin(), listaAdy.end(), arista,
                               [](const AristaDirigida<Valor>& a, const AristaDirigida<Valor>& b) {
                                   return a.capacidad() < b.capacidad();
                               });
    listaAdy.insert(it, arista);

    ++_A;
    }

    /**
     * Comprueba si hay arista de v a w.
     */
    bool hayArista(int v, int w) const {
        for (auto a : _ady[v])
            if (a.hasta() == w) return true;
        return false;
    }

    /**
     * Devuelve la lista de adyacencia de v.
     * @throws invalid_argument si v no existe
     */
    AdysDirVal<Valor> const& ady(int v) const {
        if (v < 0 || v >= _V)
            throw std::invalid_argument("Vertice inexistente");
        return _ady[v];
    }

    /**
     * Devuelve el grafo dirigido inverso.
     */
    DigrafoValorado<Valor> inverso() const {
        DigrafoValorado<Valor> inv(_V);
        for (auto v = 0; v < _V; ++v) {
            for (auto a : _ady[v]) {
                inv.ponArista({ a.hasta(), a.desde(), a.valor() });
            }
        }
        return inv;
    }

    /**
     * Muestra el grafo en el stream de salida o
     */
    void print(std::ostream& o = std::cout) const {
        o << _V << " vértices, " << _A << " aristas\n";
        for (auto v = 0; v < _V; ++v) {
            o << v << ": ";
            for (auto a : _ady[v]) {
                o << a << " ";
            }
            o << "\n";
        }
    }

private:
    int _V;   // número de vértices
    int _A;   // número de aristas
    std::vector<AdysDirVal<Valor>> _ady;   // vector de listas de adyacentes

};

/**
 * Para mostrar grafos por la salida estándar.
 */
template <typename Valor>
inline std::ostream& operator<<(std::ostream& o, const DigrafoValorado<Valor>& g) {
    g.print(o);
    return o;
}



template < typename Valor >
class FlujoCosteMinimo {
private:
    const Valor INF = std::numeric_limits<Valor>::max();
    int origen;
    vector<int> cost;
    vector<Valor> flujo_aux;
    vector<int> flujo_nodo;
    IndexPQ<Valor> pq;
    void relajar(AristaDirigida<Valor> a) {
        int v = a.desde(), w = a.hasta();

        int flujo; // cantidad de flujo se intenta enviar
        if (flujo_nodo[v] + a.flujo() >= a.capacidad()) flujo = a.capacidad() - a.flujo();
        else flujo = flujo_nodo[v];

        if (cost[w] > (cost[v] + (flujo*a.coste()))) { // comprueba si es mejor coste
            cost[w] = cost[v] + (flujo * a.coste());   // actualiza el coste
            flujo_nodo[v] -= flujo;                    // actualiza el flujo que le queda el nodo por enviar
            flujo_nodo[w] += flujo;                    // actualiza el flujo que recibe en otro nodo
            a.addFlujo(flujo);                         // actualiza el flujo

            pq.update(w, cost[w]);
        }
    }

public:
    FlujoCosteMinimo(DigrafoValorado<Valor> const& g, vector<int>& flujo, int orig) : origen(orig),
        cost(g.V(), INF), flujo_aux(g.V(), INF), pq(g.V()) {

        flujo_nodo = flujo;

        cost[origen] = 0;
        pq.push(origen, 0);
        while (!pq.empty()) { // quedan vertices por revisar de la cola
            
            int v = pq.top().elem; pq.pop(); // se saca el vertice con el menor coste que haya en la cola
            
            for (auto a : g.ady(v))         // se revisan las aristas del vertice si hay flujo que repartir
                if(flujo_nodo[v] > 0) relajar(a);
        }
    }
    bool hayCamino(int v) const { return cost[v] != INF; }
    Valor coste(int v) const { return cost[v]; }
};

bool resuelveCaso() {
    // leer los datos de la entrada
    DigrafoValorado<int> g(cin, 0);     // grafo
    

    if (!std::cin)  // fin de la entrada
        return false;

    vector<int> flujo_nodo(g.V(), 0);   // guarda el flujo que hay en cada nodo
    for (int i = 0; i < g.V(); i++) {
        cin >> flujo_nodo[i];
    }

    FlujoCosteMinimo<int> f (g, flujo_nodo, 0);

    // escribir la solución 
    cout << f.coste(g.V()-1) << endl;

    return true;
}

//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    if (!in.is_open())
        std::cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    std::cout << "Pulsa Intro para salir..." << std::flush;
    std::cin.get();
#endif
    return 0;
}
