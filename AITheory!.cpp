#include <iostream>     
#include <vector>       // for dynamic arrays (nodes, edges, paths)
#include <utility>      // for std::pair used in graph initialization
#include <queue>        // for priority_queue (used in Dijkstra / Yen’s if needed)
#include <limits>       // for numeric limits (like infinity in pathfinding)
#include <cmath>        // for math functions (log, exp if needed)
#include <algorithm>    // for sorting paths (K-longest, reliability ranking)
#include <iomanip>      // for controlling output precision (fixed, setprecision)

using namespace std;

// Edge structure: stores a connection to another node and its probability (price)
struct Edge {
    int connectedNode;   // destination node index
    double price;        // probability of successful connection
};

// Node structure: represents a node in the graph
struct Node {
    bool isVisited = false;     // helper flag for DFS
    int nodeNumber = 0;         // unique identifier of node
    vector<Edge> edges;         // all outgoing edges from this node
};

// GraphData structure: wraps all nodes into a single graph
struct GraphData {
    vector<Node> nodes; // list of all nodes in the graph
};

// forward declaration for graph display
void DisplayGraph(const GraphData& g);

// Initializes the graph with hardcoded edges
GraphData InitializeNodes() {
    // Each row represents a node, and pairs are (destination, probability)
    vector<vector<pair<int, double>>> graph = {
        {}, // node 0 unused for convenience (nodes start from 1)
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
    graphData.nodes.resize(graph.size());

    for (int i = 1; i < (int)graph.size(); i++) {
        graphData.nodes[i].nodeNumber = i;

        for (int j = 0; j < (int)graph[i].size(); j++) {
            Edge e;
            e.connectedNode = graph[i][j].first;
            e.price = graph[i][j].second;
            graphData.nodes[i].edges.push_back(e);
        }
    }
    return graphData;
}

// Function to run DFS to find all possible paths from start to target
void DFSAllPaths(const GraphData& g, int u, int target,
    vector<int>& path, vector<vector<int>>& allPaths,
    vector<bool>& visited) {
    if (u == target) {
        allPaths.push_back(path);
        return;
    }
    visited[u] = true;

    for (const auto& e : g.nodes[u].edges) {
        int v = e.connectedNode;
        if (!visited[v]) {
            path.push_back(v);
            DFSAllPaths(g, v, target, path, allPaths, visited);
            path.pop_back();
        }
    }
    visited[u] = false; //hello world
}

// Compute total reliability of a path (additive version)
double PathReliability(const GraphData& g, const vector<int>& path) {
    double prob = 0.0; // reliability starts at 0 (neutral for addition)
    for (size_t i = 0; i + 1 < path.size(); i++) {
        int u = path[i];
        int v = path[i + 1];
        for (const auto& e : g.nodes[u].edges) {
            if (e.connectedNode == v) {
                prob += e.price; // add probability instead of multiply
                break;
            }
        }
    }
    return prob;
}

// Get K-longest paths (sorted by reliability ascending)
vector<vector<int>> KLongestPaths(const GraphData& g, int start, int target, int K) {
    vector<vector<int>> allPaths;
    vector<int> path = { start };
    vector<bool> visited(g.nodes.size(), false);

    DFSAllPaths(g, start, target, path, allPaths, visited);

    sort(allPaths.begin(), allPaths.end(), [&](const vector<int>& a, const vector<int>& b) {
        return PathReliability(g, a) < PathReliability(g, b);
        });

    if ((int)allPaths.size() > K) {
        allPaths.resize(K);
    }
    return allPaths;
}

// Utility function to display the graph connections
void DisplayGraph(const GraphData& g) {
    for (int i = 1; i < (int)g.nodes.size(); i++) {
        cout << "Node " << g.nodes[i].nodeNumber << " -> ";

        if (g.nodes[i].edges.empty()) {
            cout << "(no connections)";
        }
        else {
            for (const auto& e : g.nodes[i].edges) {
                cout << "Node " << e.connectedNode << " (p=" << e.price << ") ";
            }
        }
        cout << "\n";
    }
}

int main() {
    GraphData graphData = InitializeNodes();

    DisplayGraph(graphData);

    int start = 1;
    int target = 10;

    // ------------------------------------------------------------------
    // Shortest path calculation (highest additive reliability)
    // Instead of Dijkstra (which uses logs), we brute-force all paths
    // and choose the one with the maximum additive reliability.
    // ------------------------------------------------------------------
    vector<vector<int>> allPaths;
    vector<int> path = { start };
    vector<bool> visited(graphData.nodes.size(), false);
    DFSAllPaths(graphData, start, target, path, allPaths, visited);

    // find path with maximum additive reliability
    double bestReliability = -1.0;
    vector<int> bestPath;

    for (auto& p : allPaths) {
        double r = PathReliability(graphData, p);
        if (r > bestReliability) {
            bestReliability = r;
            bestPath = p;
        }
    }

    cout << "\nShortest Path (most reliable, additive) from "
        << start << " to " << target << ":\n";
    if (!bestPath.empty()) {
        cout << "Path: ";
        for (size_t i = 0; i < bestPath.size(); i++) {
            cout << bestPath[i] << (i + 1 < bestPath.size() ? " -> " : "");
        }
        cout << "\nReliability: " << fixed << setprecision(6) << bestReliability << "\n";
    }
    else {
        cout << "No path exists.\n";
    }

    // Note: Longest / Second Longest still work via KLongestPaths,
    // but are not called here anymore.

    return 0;
}
