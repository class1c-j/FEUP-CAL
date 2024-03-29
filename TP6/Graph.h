/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include <algorithm>
#include <iostream>


template<class T>
class Edge;

template<class T>
class Graph;

template<class T>
class Vertex;

#define INF std::numeric_limits<double>::max()
const double MAX_DIST = INF;


/************************* Vertex  **************************/

template<class T>
class Vertex {
    T info;                        // content of the vertex
    std::vector<Edge<T> > adj;        // outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0;        // required by MutablePriorityQueue

    bool visited = false;        // auxiliary field
    bool processing = false;    // auxiliary field

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);

    T getInfo() const;

    double getDist() const;

    Vertex *getPath() const;

    bool operator<(Vertex<T> &vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;

    friend class MutablePriorityQueue<Vertex<T>>;
};


template<class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> &vertex) const {
    return this->dist < vertex.dist;
}

template<class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template<class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template<class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template<class T>
class Edge {
    Vertex<T> *dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);

    friend class Graph<T>;

    friend class Vertex<T>;
};

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template<class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    double **adjacencyMatrix;
    int **dp;

public:
    Vertex<T> *findVertex(const T &in) const;

    size_t findVertexIdx(T info) const;

    bool addVertex(const T &in);

    bool addEdge(const T &sourc, const T &dest, double w);

    int getNumVertex() const;

    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);

    void dijkstraShortestPath(const T &s);

    void bellmanFordShortestPath(const T &s);

    std::vector<T> getPath(const T &origin, const T &dest) const;

    // Fp06 - all pairs
    void floydWarshallShortestPath();

    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2, w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    for (Vertex<T> *vertex : this->vertexSet) {
        vertex->dist = MAX_DIST;
        vertex->path = NULL;
    }
    std::queue<Vertex<T> *> vertexQueue;
    this->findVertex(orig)->dist = 0;
    vertexQueue.push(this->findVertex(orig));

    while (!vertexQueue.empty()) {
        Vertex<T> *v = vertexQueue.front();
        vertexQueue.pop();
        for (Edge<T> edge : v->adj) {
            if (edge.dest->dist == MAX_DIST) {
                edge.dest->dist = v->dist + 1;
                edge.dest->path = v;
                vertexQueue.push(edge.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    for (Vertex<T> *vertex : this->vertexSet) {
        vertex->dist = MAX_DIST;
        vertex->path = NULL;
    }
    Vertex<T> *source = findVertex(origin);
    if (source == nullptr) return;
    source->dist = 0;
    MutablePriorityQueue<Vertex<T> > q;
    q.insert(source);
    while (!q.empty()) {
        Vertex<T> *vertex = q.extractMin();
        for (Edge<T> edge : vertex->adj) {
            double oldDist = edge.dest->dist;
            if (edge.dest->dist > vertex->dist + edge.weight) {
                edge.dest->dist = vertex->dist + edge.weight;
                edge.dest->path = vertex;
                if (oldDist == MAX_DIST) {
                    q.insert(edge.dest);
                } else {
                    q.decreaseKey(edge.dest);
                }
            }
        }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    for (Vertex<T> *vertex : this->vertexSet) {
        vertex->dist = MAX_DIST;
        vertex->path = NULL;
    }
    Vertex<T> *source = findVertex(orig);
    if (source == nullptr) return;
    source->dist = 0;
    for (int i = 1; i < this->vertexSet.size(); ++i) {
        for (Vertex<T> *vertex : this->vertexSet) {
            for (Edge<T> edge : vertex->adj) {
                if (edge.dest->dist > vertex->dist + edge.weight) {
                    edge.dest->dist = vertex->dist + edge.weight;
                    edge.dest->path = vertex;
                }
            }
        }
    }
    for (Vertex<T> *vertex : this->vertexSet) {
        for (Edge<T> edge : vertex->adj) {
            if (vertex->dist + edge.weight < edge.dest->dist) {
                std::cerr << "there are cycles of negative weight\n";
            }
        }
    }
}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const {
    std::vector<T> res;
    Vertex<T> *v = this->findVertex(dest);

    if (v == nullptr || v->dist == INF) {
        return res;
    }
    res.push_back(v->info);
    while (true) {
        res.push_back(v->path->info);
        if (v->path->info == origin) break;
        v = v->path;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    size_t n = vertexSet.size();
    /*for (int i = 0; i < n; ++i) {
        delete [] adjacencyMatrix[i];
        delete [] dp[i];
    }
    delete [] adjacencyMatrix;
    delete [] dp;*/
    adjacencyMatrix = new double *[n];
    dp = new int *[n];
    for (size_t i = 0; i < n; ++i) {
        adjacencyMatrix[i] = new double[n];
        dp[i] = new int[n];
        for (size_t j = 0; j < n; ++j) {
            adjacencyMatrix[i][j] = i == j ? 0 : INF;
            dp[i][j] = -1;
        }
        for (Edge<T> edge : this->vertexSet.at(i)->adj) {
            size_t j = findVertexIdx(edge.dest->info);
            adjacencyMatrix[i][j] = edge.weight;
            dp[i][j] = i;
        }
    }
    for (size_t k = 0; k < n; ++k) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (adjacencyMatrix[i][k] == INF || adjacencyMatrix[k][j] == INF) continue;
                double newDist = adjacencyMatrix[i][k] + adjacencyMatrix[k][j];
                if (newDist < adjacencyMatrix[i][j]) {
                    adjacencyMatrix[i][j] = newDist;
                    dp[i][j] = dp[k][j];
                }
            }
        }
    }

}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const {
    std::vector<T> res;
    int i = findVertexIdx(orig);
    int j = findVertexIdx(dest);
    if (i == -1 || j == -1 || adjacencyMatrix[i][j] == INF) { // missing or disconnected
        return res;
    }
    for (; j != -1; j = dp[i][j]) {
        res.push_back(vertexSet[j]->info);
    }
    reverse(res.begin(), res.end());
    return res;
}

template<class T>
size_t Graph<T>::findVertexIdx(T info) const {
    for (size_t i = 0; i < this->vertexSet.size(); ++i) {
        if (this->vertexSet.at(i)->info == info) {
            return i;
        }
    }
    return -1;
}


#endif /* GRAPH_H_ */
