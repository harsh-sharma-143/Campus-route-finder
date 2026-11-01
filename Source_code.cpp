#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define MAX_LOCATIONS 10

class CampusGraph {
private:
    int V;
    string locations[MAX_LOCATIONS];
    int adj_list[MAX_LOCATIONS][MAX_LOCATIONS];
    int degree[MAX_LOCATIONS];
    int adj_matrix[MAX_LOCATIONS][MAX_LOCATIONS];

    int getIndex(string loc) {
        for (int i = 0; i < V; i++) {
            if (locations[i] == loc) return i;
        }
        return -1;
    }

public:
    CampusGraph(vector<string> locs) {
        V = locs.size();
        for (int i = 0; i < V; i++) {
            locations[i] = locs[i];
            degree[i] = 0;
            for (int j = 0; j < MAX_LOCATIONS; j++) {
                adj_matrix[i][j] = 0;
            }
        }
    }

    void addEdge(string u, string v) {
        int uIdx = getIndex(u);
        int vIdx = getIndex(v);

        if (uIdx != -1 && vIdx != -1) {
            // Adjacency List update
            adj_list[uIdx][degree[uIdx]++] = vIdx;
            adj_list[vIdx][degree[vIdx]++] = uIdx;

            // Adjacency Matrix update
            adj_matrix[uIdx][vIdx] = 1;
            adj_matrix[vIdx][uIdx] = 1;
        }
    }

    void displayAdjacencyList() {
        cout << "\n--- Adjacency List Representation ---\n";
        for (int i = 0; i < V; i++) {
            cout << locations[i] << " [" << i << "] -> ";
            if (degree[i] == 0) {
                cout << "None (Isolated)";
            } else {
                for (int j = 0; j < degree[i]; j++) {
                    cout << locations[adj_list[i][j]];
                    if (j < degree[i] - 1) cout << ", ";
                }
            }
            cout << "\n";
        }
    }

    void displayAdjacencyMatrix() {
        cout << "\n--- Adjacency Matrix Representation ---\n    ";
        for (int i = 0; i < V; i++) cout << i << " ";
        cout << "\n";
        for (int i = 0; i < V; i++) {
            cout << i << " | ";
            for (int j = 0; j < V; j++) {
                cout << adj_matrix[i][j] << " ";
            }
            cout << " (" << locations[i] << ")\n";
        }
    }

    void findShortestRouteBFS(string src, string dst) {
        int srcIdx = getIndex(src);
        int dstIdx = getIndex(dst);

        if (srcIdx == -1 || dstIdx == -1) {
            cout << "\nError: Invalid source or destination location!\n";
            return;
        }

        bool visited[MAX_LOCATIONS] = {false};
        int parent[MAX_LOCATIONS];
        for (int i = 0; i < V; i++) parent[i] = -1;

        queue<int> q;
        visited[srcIdx] = true;
        q.push(srcIdx);

        bool found = false;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            if (curr == dstIdx) {
                found = true;
                break;
            }

            for (int i = 0; i < degree[curr]; i++) {
                int neighbor = adj_list[curr][i];
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = curr;
                    q.push(neighbor);
                }
            }
        }

        if (!found) {
            cout << "\nNo route exists between '" << src << "' and '" << dst << "'.\n";
            return;
        }

        vector<int> path;
        int curr = dstIdx;
        while (curr != -1) {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        cout << "\nShortest Route (" << path.size() - 1 << " edges):\n";
        for (size_t i = 0; i < path.size(); i++) {
            cout << locations[path[i]];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    void dfsReachability(string start) {
        int startIdx = getIndex(start);
        if (startIdx == -1) {
            cout << "\nLocation not found!\n";
            return;
        }

        bool visited[MAX_LOCATIONS] = {false};
        
        vector<int> stack;
        stack.push_back(startIdx);
        visited[startIdx] = true;

        while (!stack.empty()) {
            int curr = stack.back();
            stack.pop_back();

            for (int i = 0; i < degree[curr]; i++) {
                int neighbor = adj_list[curr][i];
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push_back(neighbor);
                }
            }
        }

        cout << "\n--- DFS Reachability Analysis from '" << start << "' ---\n";
        cout << "Reachable Locations: ";
        for (int i = 0; i < V; i++) {
            if (visited[i]) cout << locations[i] << "  ";
        }
        cout << "\nDisconnected Locations: ";
        bool hasDisconnected = false;
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                cout << locations[i] << "  ";
                hasDisconnected = true;
            }
        }
        if (!hasDisconnected) cout << "None";
        cout << "\n";
    }

    void runTestCases() {
        cout << "\n=== RUNNING PREDEFINED TEST CASES ===\n";

        cout << "\n[Test Case 1: Direct Route]";
        findShortestRouteBFS("Main Gate", "Block 1");

        cout << "\n[Test Case 2: Multi-stop Route]";
        findShortestRouteBFS("Main Gate", "Auditorium");

        cout << "\n[Test Case 3: Unreachable Destination]";
        findShortestRouteBFS("Main Gate", "Hostel");
    }
};

int main() {
    vector<string> locs = {
        "Main Gate", "Library", "Block 1", "Block 2", 
        "Cafeteria", "Auditorium", "Parking", "Lab", "Hostel"
    };

    CampusGraph graph(locs);

    // Adding campus edges
    graph.addEdge("Main Gate", "Library");
    graph.addEdge("Main Gate", "Parking");
    graph.addEdge("Library", "Block 1");
    graph.addEdge("Library", "Cafeteria");
    graph.addEdge("Block 1", "Block 2");
    graph.addEdge("Block 1", "Lab");
    graph.addEdge("Block 2", "Auditorium");
    graph.addEdge("Cafeteria", "Auditorium");
    graph.addEdge("Parking", "Lab");
    // "Hostel" is left intentionally disconnected

    int choice;
    while (true) {
        cout << "\n==========================================";
        cout << "\n      CAMPUS ROUTE FINDER (GRAPH)        ";
        cout << "\n==========================================";
        cout << "\n1. Display Adjacency List & Matrix";
        cout << "\n2. Find Shortest Route (BFS)";
        cout << "\n3. Check Reachable Locations (DFS)";
        cout << "\n4. Run Mandatory Test Cases";
        cout << "\n5. Exit";
        cout << "\nEnter choice (1-5): ";
        
        if (!(cin >> choice)) break;

        if (choice == 1) {
            graph.displayAdjacencyList();
            graph.displayAdjacencyMatrix();
        } else if (choice == 2) {
            cin.ignore();
            string src, dst;
            cout << "Enter Source Location: ";
            getline(cin, src);
            cout << "Enter Destination Location: ";
            getline(cin, dst);
            graph.findShortestRouteBFS(src, dst);
        } else if (choice == 3) {
            cin.ignore();
            string start;
            cout << "Enter Starting Location: ";
            getline(cin, start);
            graph.dfsReachability(start);
        } else if (choice == 4) {
            graph.runTestCases();
        } else if (choice == 5) {
            cout << "Exiting Campus Route Finder.\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}