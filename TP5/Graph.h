/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <algorithm>

template<class T>
class Edge;

template<class T>
class Graph;

template<class T>
class Vertex;


/****************** Provided structures  ********************/

template<class T>
class Vertex {
    T info;                // contents
    std::vector<Edge<T> > adj;  // list of outgoing edges
    bool visited;          // auxiliary field used by dfs and bfs
    bool processing;       // auxiliary field used by isDAG
    int indegree;          // auxiliary field used by topsort

    void addEdge(Vertex<T> *dest, double w);

    bool removeEdgeTo(Vertex<T> *d);

public:
    explicit Vertex(T in);

    friend class Graph<T>;
};

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
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    void dfsVisit(Vertex<T> *v, std::vector<T> &res) const;

    Vertex<T> *findVertex(const T &in) const;

    bool dfsIsDAG(Vertex<T> *v) const;

public:
    int getNumVertex() const;

    bool addVertex(const T &in);

    bool removeVertex(const T &in);

    bool addEdge(const T &sourc, const T &dest, double w);

    bool removeEdge(const T &sourc, const T &dest);

    std::vector<T> dfs() const;

    std::vector<T> bfs(const T &source) const;

    std::vector<T> topsort() const;

    int maxNewChildren(const T &source, T &inf) const;

    bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template<class T>
Vertex<T>::Vertex(T in): info(in) {
}

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {
}


template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != NULL) {
        return false;
    }
    auto *v = new Vertex<T>(in);
    this->vertexSet.push_back(v);
    return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    Vertex<T> *srcV = findVertex(sourc);
    if (srcV == NULL) return false;
    Vertex<T> *destV = findVertex(dest);
    if (destV == NULL) return false;
    srcV->addEdge(destV, w);
    return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    Edge<T> e(d, w);
    this->adj.push_back(e);
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> *srcV = findVertex(sourc);
    if (srcV == NULL) return false;
    Vertex<T> *destV = findVertex(dest);
    if (destV == NULL) return false;
    return srcV->removeEdgeTo(destV);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for (auto it = adj.begin(); it != adj.end(); ++it) {
        if (it->dest == d) {
            adj.erase(it);
            return true;
        }
    }
    return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); ++it) {
        if (in == (*it)->info) {
            for (auto toIt = vertexSet.begin(); toIt != vertexSet.end(); ++toIt) {
                (*toIt)->removeEdgeTo(*it);
            }
            delete *it;
            vertexSet.erase(it);
            return true;
        }
    }
    return false;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template<class T>
std::vector<T> Graph<T>::dfs() const {
    for (auto vert : vertexSet) {
        vert->visited = false;
    }
    std::vector<T> res;
    for (auto vert : vertexSet) {
        if (!vert->visited) {
            this->dfsVisit(vert, res);
        }
    }
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template<class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> &res) const {
    v->visited = true;
    res.push_back(v->info);
    for (auto e : v->adj) {
        if (!e.dest->visited) {
            dfsVisit(e.dest, res);
        }
    }
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template<class T>
std::vector<T> Graph<T>::bfs(const T &source) const {
    std::vector<T> res;
    for (auto vert : vertexSet) {
        vert->visited = false;
    }
    Vertex<T> *s = findVertex(source);
    if (s == NULL) {
        return res;
    }
    s->visited = true;
    std::queue<Vertex<T> *> Q;
    Q.push(s);
    while (!Q.empty()) {
        Vertex<T> *a = Q.front();
        res.push_back(a->info);
        Q.pop();
        for (auto edge : a->adj) {
            if (!edge.dest->visited) {
                Q.push(edge.dest);
                edge.dest->visited = true;
            }
        }
    }
    return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
std::vector<T> Graph<T>::topsort() const {
    std::vector<T> res;
    for (Vertex<T> *vertex : vertexSet) {
        vertex->indegree = 0;
    }
    for (Vertex<T> *v : vertexSet) {
        for (Edge<T> w : v->adj) {
            w.dest->indegree++;
        }
    }
    std::queue<Vertex<T> *> queue{
    };
    for (Vertex<T> *vertex : vertexSet) {
        if (vertex->indegree == 0) {
            queue.push(vertex);
        }
    }
    while (!queue.empty()) {
        Vertex<T> *v = queue.front();
        queue.pop();
        res.push_back(v->info);
        for (Edge<T> edge : v->adj) {
            edge.dest->indegree--;
            if (edge.dest->indegree == 0) {
                queue.push(edge.dest);
            }
        }
    }
    if (res.size() != vertexSet.size()) {
        return {
        };
    }
    return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template<class T>
int Graph<T>::maxNewChildren(const T &source, T &inf) const {
    int maxChildren = 0;
    Vertex<T> *vertex = findVertex(source);
    if (vertex == nullptr) {
        inf = T{
        };
        return 0;
    }

    for (Vertex<T> *v : vertexSet) {
        v->visited = false;
    }

    vertex->visited = true;
    std::queue<Vertex<T> *> toVisit{};
    toVisit.push(vertex);

    while (!toVisit.empty()) {
        Vertex<T> *found = toVisit.front();
        toVisit.pop();
        int childCount = 0;
        for (Edge<T> adjEdge : found->adj) {
            if (!adjEdge.dest->visited) {
                toVisit.push(adjEdge.dest);
                adjEdge.dest->visited = true;
                ++childCount;
            }
        }
        if (childCount > maxChildren) {
            maxChildren = childCount;
            inf = found->info;
        }
    }

    return maxChildren;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template<class T>
bool Graph<T>::isDAG() const {
    for (Vertex<T> *vertex : vertexSet) {
        vertex->processing = false;
        vertex->visited = false;
    }
    return std::all_of(vertexSet.begin(), vertexSet.end(), [this](Vertex<T>* vertex){
        return vertex->visited || dfsIsDAG(vertex);
    });
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template<class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    v->processing = true;
    v->visited = true;
    for (Edge<T> adjEdge : v->adj) {
        if (adjEdge.dest->processing) return false;
        if (!adjEdge.dest->visited) {
            if (!dfsIsDAG(adjEdge.dest)) return false;
        }
    }
    v->processing = false;
    return true;
}

#endif /* GRAPH_H_ */
