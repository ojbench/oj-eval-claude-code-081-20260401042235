#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 3005;

int n, m;
vector<pair<int, int>> edges;
vector<int> adj[MAXN];
int cap[MAXN][MAXN];

// Edmonds-Karp with early termination
int maxFlow(int s, int t) {
    // Copy capacity matrix
    static int flow[MAXN][MAXN];
    memset(flow, 0, sizeof(flow));

    int total = 0;

    while (true) {
        // BFS to find shortest augmenting path
        static int parent[MAXN];
        memset(parent, -1, sizeof(parent));
        parent[s] = s;

        queue<int> q;
        q.push(s);

        while (!q.empty() && parent[t] == -1) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (parent[v] == -1 && cap[u][v] > flow[u][v]) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (parent[t] == -1) break;

        // Find bottleneck
        int bottleneck = 1;

        // Augment flow
        int v = t;
        while (v != s) {
            int u = parent[v];
            flow[u][v] += bottleneck;
            flow[v][u] -= bottleneck;
            v = u;
        }

        total += bottleneck;

        // Early exit: max flow is at most 3 (min degree)
        if (total >= 3) break;
    }

    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    memset(cap, 0, sizeof(cap));

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        cap[a][b] = 1;
        cap[b][a] = 1;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    long long answer = 0;

    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            answer += maxFlow(a, b);
        }
    }

    cout << answer << endl;

    return 0;
}
