#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

int n, m;
int capacity[MAXN][MAXN];
int flow[MAXN][MAXN];
vector<int> adj[MAXN];

// BFS to find augmenting path from source to sink
bool bfs(int source, int sink, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[source] = source;

    queue<pair<int, int>> q;
    q.push({source, INF});

    while (!q.empty()) {
        int u = q.front().first;
        int curr_flow = q.front().second;
        q.pop();

        for (int v : adj[u]) {
            if (parent[v] == -1 && capacity[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                int new_flow = min(curr_flow, capacity[u][v] - flow[u][v]);
                if (v == sink) {
                    return true;
                }
                q.push({v, new_flow});
            }
        }
    }

    return false;
}

// Edmonds-Karp algorithm for maximum flow
int maxFlow(int source, int sink) {
    // Reset flow for this computation
    memset(flow, 0, sizeof(flow));

    int total_flow = 0;
    vector<int> parent(n + 1);

    while (bfs(source, sink, parent)) {
        // Find minimum residual capacity along the path
        int path_flow = INF;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, capacity[u][v] - flow[u][v]);
        }

        // Update flows along the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += path_flow;
            flow[v][u] -= path_flow;
        }

        total_flow += path_flow;
    }

    return total_flow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    memset(capacity, 0, sizeof(capacity));

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        // Bidirectional edge with capacity 1
        capacity[a][b] = 1;
        capacity[b][a] = 1;

        // Build adjacency list
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    long long answer = 0;

    // Calculate max flow for all pairs (a, b) where a < b
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            int max_flow = maxFlow(a, b);
            answer += max_flow;
        }
    }

    cout << answer << endl;

    return 0;
}
