#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 3005;

int n, m;
struct Edge {
    int to, cap, flow;
};
vector<Edge> edges;
vector<int> graph[MAXN];

void addEdge(int from, int to, int cap) {
    graph[from].push_back(edges.size());
    edges.push_back({to, cap, 0});
    graph[to].push_back(edges.size());
    edges.push_back({from, cap, 0});
}

// Very fast BFS-based max flow for unit capacities
int maxFlow(int s, int t) {
    // Reset flows
    for (auto& e : edges) {
        e.flow = 0;
    }

    int totalFlow = 0;

    // Find up to 3 augmenting paths (bounded by degree)
    for (int iter = 0; iter < 3; iter++) {
        vector<int> parent(n + 1, -1);
        vector<int> parentEdge(n + 1, -1);
        queue<int> q;

        parent[s] = s;
        q.push(s);

        bool found = false;
        while (!q.empty() && !found) {
            int u = q.front();
            q.pop();

            if (u == t) {
                found = true;
                break;
            }

            for (int idx : graph[u]) {
                Edge& e = edges[idx];
                if (parent[e.to] == -1 && e.cap > e.flow) {
                    parent[e.to] = u;
                    parentEdge[e.to] = idx;
                    q.push(e.to);
                }
            }
        }

        if (!found) break;

        // Augment path
        int v = t;
        while (v != s) {
            int idx = parentEdge[v];
            edges[idx].flow++;
            edges[idx ^ 1].flow--;
            v = parent[v];
        }

        totalFlow++;
    }

    return totalFlow;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        addEdge(a, b, 1);
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
