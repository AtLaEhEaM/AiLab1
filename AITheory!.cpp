#include<iostream>
#include<vector>
using namespace std;


struct Node { //NoDe
	int nodeNumber = 0; //assign node number to keep track of shit
	vector<Edge> edges; //since all edges are outgoing we do not need seperation for in and out
};

struct Edge {
	int connectedNode;
    int price;
};

struct GraphData { //since values are provided beforehand we can hard code them >:D
    vector<Node> nodes;
};

GraphData InitializeNodes() {
    vector<vector<pair<int, double>>> graph = {
        {}, // node 0 unused
        { {2, 0.2}, {3, 0.9}, {9, 0.9} },
        { {7, 0.8}, {3, 0.1} },
        { {5, 0.1}, {4, 0.1}, {7, 0.35} },
        { {8, 0.25} },
        { {4, 0.1} },
        { {10, 0.5} },
        { {6, 0.35}, {5, 0.1} },
        { {10, 0.4} },
        { {4, 0.1} },
        {}
    };

    GraphData graphData;
    graphData.nodes.resize(graph.size()); // allocate all nodes

    for (int i = 1; i < graph.size(); i++) {
        graphData.nodes[i].nodeNumber = i;

        for (int j = 0; j < graph[i].size(); j++) {
            Edge e;
            e.connectedNode = graph[i][j].first;   // destination node
            e.price = graph[i][j].second;  // probability
            graphData.nodes[i].edges.push_back(e);
        }
    }

    return graphData;
}



