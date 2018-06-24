#include <bits/stdc++.h>

using namespace std;

const size_t MAX_N = 1000;
const int INF = (int)1e9 + 21;

struct Edge {
    size_t u;
    int cap;
    int cost;

    int flow;
    size_t rev;

    size_t id;

    Edge(size_t _u, size_t _id, size_t _rev, int _cap, int _cost) {
        u = _u, cap = _cap, cost = _cost, flow = 0;
        rev = _rev, id = _id;
    }
};

size_t n, m;
vector<vector<size_t> > graph;
vector<Edge> edges;
vector<int> phi;
vector<int> used;
vector<int> dist;
vector<size_t> which;
int cnt = 0;
size_t s, t;

void dijkstraHeap() {
    ++cnt;
    dist[s] = 0;

    set<pair<int, size_t> > heap;
    heap.insert({dist[s], s});

    pair<int, size_t> elem;
    size_t v, u;
    int d, cost;
    while(!heap.empty()) {
        auto elem = *heap.begin();
        heap.erase(heap.begin());

        d = elem.first;
        v = elem.second;

        if (used[v] == cnt) {
            continue;
        }
        used[v] = cnt;

        for (size_t edge : graph[v]) {
            if (edges[edge].cap == edges[edge].flow) {
                continue;
            }

            u = edges[edge].u;
            cost = edges[edge].cost;

            if ((used[u] != cnt) && (dist[u] > d + cost + phi[v] - phi[u])) {
                dist[u] = d + cost + phi[v] - phi[u];
                which[u] = edges[edge].rev;
                heap.insert({dist[u], u});
            }
        }
    }
}

int minCostMaxFlow(int k) {
    int res = 0;
    int flow = 0;

    while (flow < k) {
        dist.clear();
        dist.resize(n + 1, INF);
        which.clear();
        which.resize(n + 1);
        dijkstraHeap();

        if (used[t] != cnt) {
            return -1;
        }

        for (size_t i = 1; i != n + 1; ++i) {
            if (used[i] == cnt) {
                phi[i] += dist[i];
            }
        }

        int extra = INF;
        int v = t;
        while (v != s) {
            extra = min(extra, edges[edges[which[v]].rev].cap - edges[edges[which[v]].rev].flow);
            v = edges[which[v]].u;
        }
        v = t;
        while (v != s) {
            edges[which[v]].flow -= extra;
            edges[edges[which[v]].rev].flow += extra;
            res += edges[edges[which[v]].rev].cost * extra;
            v = edges[which[v]].u;
        }

        flow += extra;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);

    int k;
    cin >> n >> m >> k;
    s = 1, t = n;

    used.resize(n + 1);
    graph.resize(n + 1);
    phi.resize(n + 1);

    size_t fr, to, cost;
    for (size_t i = 0; i != m; ++i) {
        cin >> fr >> to >> cost;

        graph[fr].push_back(edges.size());
        edges.push_back(Edge(to, i, edges.size() + 1, 1, cost));
        graph[to].push_back(edges.size());
        edges.push_back(Edge(fr, i, edges.size() - 1, 0, -cost));

        graph[to].push_back(edges.size());
        edges.push_back(Edge(fr, i, edges.size() + 1, 1, cost));
        graph[fr].push_back(edges.size());
        edges.push_back(Edge(to, i, edges.size() - 1, 0, -cost));
    }

    int ans = minCostMaxFlow(k);
    if (ans == -1) {
        cout << "-1\n";
        return 0;
    }

    cout.precision(21);
    cout << (double)ans / (double)k << endl;

    size_t v;
    while (k != 0) {
        --k;
        vector<size_t> path;

        v = t;
        while (v != s) {
            for (size_t u : graph[v]) {
                if (edges[u].flow == -1) {
                    edges[u].flow = 0;
                    v = edges[u].u;
                    path.push_back(edges[u].id + 1);

                    break;
                }
            }
        }

        cout << path.size() << " ";
        reverse(path.begin(), path.end());
        for (auto edge : path) {
            cout << edge << " ";
        }
        cout << endl;
    }

    return 0;
}
