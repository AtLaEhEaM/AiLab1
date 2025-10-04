#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <iomanip>
using namespace std;

// Edge structure
struct Edge {
    int connectedNode;   // destination node
    double price;        // probability (0 to 1)
};

// Node structure
struct Node {
    int nodeNumber = 0;
    vector<Edge> edges;
};

// Graph wrapper
struct GraphData {
    vector<Node> nodes;
};

// Build example graph
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
    graphData.nodes.resize(graph.size());

    for (int i = 1; i < (int)graph.size(); i++) {
        graphData.nodes[i].nodeNumber = i;
        for (auto& p : graph[i]) {
            graphData.nodes[i].edges.push_back({ p.first, p.second });
        }
    }
    return graphData;
}

// Dijkstra with multiplicative probability (using -log transform)
pair<double, vector<int>> DijkstraMostReliable(const GraphData& g, int start, int target) {
    int n = g.nodes.size();
    vector<double> dist(n, numeric_limits<double>::infinity());
    vector<int> parent(n, -1);

    using State = pair<double, int>;
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[start] = 0.0;
    pq.push({ 0.0, start });

    while (!pq.empty()) {
        auto [currDist, u] = pq.top();
        pq.pop();

        if (currDist > dist[u]) continue;

        for (auto& edge : g.nodes[u].edges) {
            int v = edge.connectedNode;
            double w = -log(edge.price);

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({ dist[v], v });
            }
        }
    }

    vector<int> path;
    if (dist[target] == numeric_limits<double>::infinity()) {
        return { 0.0, path };
    }

    for (int v = target; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    double probability = exp(-dist[target]);
    return { probability, path };
}

// Separate function to display path result
void DisplayPathResult(int start, int target, double prob, const vector<int>& path) {
    cout << "\nMost Reliable Path from " << start << " to " << target << ":\n";
    if (!path.empty()) {
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << (i + 1 < path.size() ? " -> " : "");
        }
        cout << "\nReliability: " << fixed << setprecision(6) << prob << "\n";
    }
    else {
        cout << "No path exists.\n";
    }
}

int main() {
    GraphData graphData = InitializeNodes();

    int start = 1;
    int target = 10;

   /* auto [prob, path] = DijkstraMostReliable(graphData, start, target);

    DisplayPathResult(start, target, prob, path);*/

    start = (rand() % 9) + 1;
    target = (rand() % 10) + start;
    cout << start << " " << target;

    auto [prob, path] = DijkstraMostReliable(graphData, start, target);

    DisplayPathResult(start, target, prob, path);

    return 0;
}
