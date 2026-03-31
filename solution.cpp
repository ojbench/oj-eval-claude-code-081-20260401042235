#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, flow;
};

int n, m;
vector<Edge> edges;
vector<int> graph[MAXN];
int level[MAXN];
int iter[MAXN];

void addEdge(int from, int to, int cap) {
    graph[from].push_back(edges.size());
    edges.push_back({to, cap, 0});
    graph[to].push_back(edges.size());
    edges.push_back({from, cap, 0});
}

bool bfs(int s, int t) {
    fill(level, level + n + 1, -1);
    queue<int> q;
    level[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (v == t) return true; // Early exit when we reach target
        for (int id : graph[v]) {
            if (level[edges[id].to] < 0 && edges[id].cap > edges[id].flow) {
                level[edges[id].to] = level[v] + 1;
                q.push(edges[id].to);
            }
        }
    }

    return false;
}

int dfs(int v, int t, int pushed) {
    if (v == t || pushed == 0) return pushed;

    for (int& cid = iter[v]; cid < (int)graph[v].size(); cid++) {
        int id = graph[v][cid];
        int to = edges[id].to;

        if (level[v] + 1 != level[to] || edges[id].cap <= edges[id].flow)
            continue;

        int tr = dfs(to, t, min(pushed, edges[id].cap - edges[id].flow));
        if (tr > 0) {
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
    }

    return 0;
}

int maxFlow(int s, int t) {
    // Reset flows only for edges that were used
    for (auto& e : edges) {
        e.flow = 0;
    }

    int flow = 0;
    while (bfs(s, t)) {
        fill(iter, iter + n + 1, 0);
        while (int pushed = dfs(s, t, INF)) {
            flow += pushed;
            // Early termination: max flow bounded by min degree (at most 3)
            if (flow >= 3) return flow;
        }
    }
    return flow;
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

    // Calculate max flow for all pairs (a, b) where a < b
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            answer += maxFlow(a, b);
        }
    }

    cout << answer << endl;

    return 0;
}
