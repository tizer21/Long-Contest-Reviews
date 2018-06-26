#include <algorithm>
#include <ios>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

static constexpr int INF = std::numeric_limits<int>::max();

struct Edge {
    size_t u;
    int cap;
    int cost;

    int flow;
    size_t rev;

    size_t id;

    Edge(size_t _u, size_t _id, size_t _rev, int _cap, int _cost)
    : u(_u)
    , cap(_cap)
    , cost(_cost)
    , rev(_rev)
    , id(_id)
    , flow(0) {}
};

struct myData {
    size_t n, m;
    std::vector<std::vector<size_t> > graph;
    std::vector<Edge> edges;
    std::vector<int> phi;
    std::vector<int> used;
    std::vector<int> dist;
    std::vector<size_t> which;
    int cnt = 0;
    size_t s, t;

    myData() {}
};

void dijkstraHeap(myData &md) {
    ++md.cnt;
    md.dist[md.s] = 0;

    std::set<std::pair<int, size_t> > heap;
    heap.insert({md.dist[md.s], md.s});

    size_t v, u;
    int d, cost;
    while(!heap.empty()) {
        auto elem = *heap.begin();
        heap.erase(heap.begin());

        d = elem.first;
        v = elem.second;

        if (md.used[v] == md.cnt) {
            continue;
        }
        md.used[v] = md.cnt;

        for (size_t edge : md.graph[v]) {
            if (md.edges[edge].cap == md.edges[edge].flow) {
                continue;
            }

            u = md.edges[edge].u;
            cost = md.edges[edge].cost;

            if ((md.used[u] != md.cnt) && (md.dist[u] > d + cost + md.phi[v] - md.phi[u])) {
                md.dist[u] = d + cost + md.phi[v] - md.phi[u];
                md.which[u] = md.edges[edge].rev;
                heap.insert({md.dist[u], u});
            }
        }
    }
}

int minCostMaxFlow(myData &md, int k) {
    int res = 0;
    int flow = 0;

    while (flow < k) {
        md.dist.clear();
        md.dist.resize(md.n + 1, INF);
        md.which.clear();
        md.which.resize(md.n + 1);
        dijkstraHeap(md);

        if (md.used[md.t] != md.cnt) {
            return -1;
        }

        for (size_t i = 1; i != md.n + 1; ++i) {
            if (md.used[i] == md.cnt) {
                md.phi[i] += md.dist[i];
            }
        }

        int extra = INF;
        int v = md.t;
        while (v != md.s) {
            extra = std::min(extra, md.edges[md.edges[md.which[v]].rev].cap - md.edges[md.edges[md.which[v]].rev].flow);
            v = md.edges[md.which[v]].u;
        }
        v = md.t;
        while (v != md.s) {
            md.edges[md.which[v]].flow -= extra;
            md.edges[md.edges[md.which[v]].rev].flow += extra;
            res += md.edges[md.edges[md.which[v]].rev].cost * extra;
            v = md.edges[md.which[v]].u;
        }

        flow += extra;
    }

    return res;
}

int init(myData &md) {
    int k;
    std::cin >> md.n >> md.m >> k;
    md.s = 1, md.t = md.n;

    md.used.resize(md.n + 1);
    md.graph.resize(md.n + 1);
    md.phi.resize(md.n + 1);

    size_t fr, to, cost;
    for (size_t i = 0; i != md.m; ++i) {
        std::cin >> fr >> to >> cost;

        md.graph[fr].push_back(md.edges.size());
        md.edges.emplace_back(to, i, md.edges.size() + 1, 1, cost);
        md.graph[to].push_back(md.edges.size());
        md.edges.emplace_back(fr, i, md.edges.size() - 1, 0, -cost);

        md.graph[to].push_back(md.edges.size());
        md.edges.emplace_back(fr, i, md.edges.size() + 1, 1, cost);
        md.graph[fr].push_back(md.edges.size());
        md.edges.emplace_back(to, i, md.edges.size() - 1, 0, -cost);
    }

    return k;
}

void print(myData &md, int ans, int k) {
    std::cout.precision(21);
    std::cout << (double)ans / (double)k << "\n";

    size_t v;
    while (k != 0) {
        --k;
        std::vector<size_t> path;

        v = md.t;
        while (v != md.s) {
            for (size_t u : md.graph[v]) {
                if (md.edges[u].flow == -1) {
                    md.edges[u].flow = 0;
                    v = md.edges[u].u;
                    path.push_back(md.edges[u].id + 1);

                    break;
                }
            }
        }

        std::cout << path.size() << " ";
        std::reverse(path.begin(), path.end());
        for (auto edge : path) {
            std::cout << edge << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    myData md;

    int k = init(md);

    int ans = minCostMaxFlow(md, k);
    if (ans == -1) {
        std::cout << "-1\n";
        return 0;
    }

    print(md, ans, k);

    return 0;
}
