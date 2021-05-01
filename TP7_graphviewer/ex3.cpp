#include <fstream>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex3() {

    // Instantiate GraphViewer
    GraphViewer gv;
    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));
    // Create window
    gv.createWindow(600, 600);

    std::ifstream nodes("../TP7_graphviewer/resources/map1/nodes.txt");
    unsigned n;
    nodes >> n;
    for (int i = 0; i < n; ++i) {
        unsigned id;
        float x, y;
        nodes >> id >> x >> y;
        gv.addNode(id, sf::Vector2f(x, y));
    }
    nodes.close();

    std::ifstream edges("../TP7_graphviewer/resources/map1/edges.txt");
    unsigned e;
    edges >> e;
    for (int i = 0; i < e; ++i) {
        unsigned id, from, to;
        edges >> id >> from >> to;
        gv.addEdge(id, gv.getNode(from), gv.getNode(to));
    }
    nodes.close();

    // Join viewer thread (blocks till window closed)
    gv.join();
}

