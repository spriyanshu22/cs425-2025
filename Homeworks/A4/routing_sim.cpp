#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const int INF = 9999;


void printDVRTable(int node, const vector<vector<int> >& table, const vector<vector<int> >& nextHop) {
    cout << "Node " << node << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";
    for (int i = 0; i < table.size(); ++i) {
        cout << i << "\t" << table[node][i] << "\t";
        if (nextHop[node][i] == -1) cout << "-";
        else cout << nextHop[node][i];
        cout << endl;
    }
    cout << endl;
}

void printLSRTable(int src, const vector<int>& dist, const vector<int>& prev) {
    cout << "Node " << src << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";
    for (int i = 0; i < dist.size(); ++i) {
        if (i == src) continue;
        cout << i << "\t" << dist[i] << "\t";
        int hop = i;
        while (prev[hop] != src && prev[hop] != -1)
            hop = prev[hop];
        cout << (prev[hop] == -1 ? -1 : hop) << endl;
    }
    cout << endl;
}

void simulateDVR(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> nextHop(n, vector<int>(n, -1));


    //TODO: Complete this
    // Initializing the distance and next hop tables
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                dist[i][j] = 0;
                nextHop[i][j] = -1;
            } else if (graph[i][j] != 0 && graph[i][j] != INF) {
                dist[i][j] = graph[i][j];
                nextHop[i][j] = j;
            }
        }
    }

    bool updated;
    int iteration = 0;

    // repeatedly update the distance and next hop tables until no changes occur
    do {
        updated = false;
        
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                for (int k = 0; k < n; ++k) {
                    if (graph[u][k] != INF && dist[k][v] != INF && k != u) {
                        int newDist = graph[u][k] + dist[k][v];
                        if (newDist < dist[u][v]) {
                            dist[u][v] = newDist;
                            nextHop[u][v] = k;
                            updated = true;
                        }
                    }
                }
            }
        }

        // printing the distance vector table for each node after each iteration
        // for (int i = 0; i < n; ++i)
        //     printDVRTable(i, dist, nextHop);

    } while (updated);

    for (int i = 0; i < n; ++i)
        printDVRTable(i, dist, nextHop);
}


void simulateLSR(const vector<vector<int> >& graph) {
    int n = graph.size();
    for (int src = 0; src < n; ++src) {
        vector<int> dist(n, INF);
        vector<int> prev(n, -1);
        vector<bool> visited(n, false);
        dist[src] = 0;
        
        //TODO: Complete this
        // .first = distance, .second = node, and the priority queue dynamically contains smallest distance at the top
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        pq.push(make_pair(0, src));
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (visited[u]) continue;
            visited[u] = true;

            for (int v = 0; v < n; ++v) {
                if (graph[u][v] != INF && !visited[v]) {
                    if (dist[v] > dist[u] + graph[u][v]) {
                        dist[v] = dist[u] + graph[u][v];
                        prev[v] = u;
                        pq.push(make_pair(dist[v], v));
                    }
                }
            }
        }

        printLSRTable(src, dist, prev);
    }
}

vector<vector<int> > readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }
    
    int n;
    file >> n;
    vector<vector<int> > graph(n, vector<int>(n));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> graph[i][j];

    file.close();
    return graph;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    string filename = argv[1];
    vector<vector<int> > graph = readGraphFromFile(filename);

    cout << "\n--- Distance Vector Routing Simulation ---\n";
    simulateDVR(graph);

    cout << "\n--- Link State Routing Simulation ---\n";
    simulateLSR(graph);

    return 0;
}
