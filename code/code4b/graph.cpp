/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>     // assert
#include <limits>      // std::numeric_limits
#include <algorithm>   // std::make_heap(), std::pop_heap(), std::push_heap()

#include "graph.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(n + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge e
// update weight if edge e is present
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.from])) {
            T[e1.from].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());
}

// remove undirected edge e
void Graph::removeEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edgeRemoval = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                               [e1](const Edge &ed) { return e1.links_same_nodes(ed); });

        assert(it != end(T[e1.from]));
        T[e1.from].erase(it);  // remove edge e1
        --n;                   // decrement the counter of edges
    };

    edgeRemoval(e);
    edgeRemoval(e.reverse());
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    // *** TODO ***

    //std::vector<Edge> edges;
    //for (int i = 1; i <= size; ++i) {
    //    for (auto e : table[i]) {
    //        edges.push_back(e);
    //    }
    //}

    dist[1] = 0;
    done[1] = true;
    int v = 1;

    int totalWeight = 0;

    while ( true ) {
        int u = -1;
        for (auto& e : table[v]) {
            int u = e.to;
            int w = e.weight;

            if (!done[u] && dist[u] > w) {
                dist[u] = w;
                path[u] = v;
                
            }
        }

        int next_v = -1;
        int min = std::numeric_limits<int>::max();

        for (int u = 1; u <= size; u++) {
            if (!done[u] && dist[u] < min) {
                min = dist[u];
                next_v = u;

                
            }
        }

        if (next_v == -1) {
            break;
        }

        //v = next_v;

        done[next_v] = true;

        std::cout << path[next_v] << " -- " << next_v << " [weight: " << dist[next_v] << "]\n";
        totalWeight += dist[next_v];
        v = next_v;
    }

    std::cout << "\nTotal weight = " << totalWeight << "\n";
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***'

    std::vector<Edge> edges;


    for (int u = 1; u <= size; ++u) {
        for (const auto& e : table[u]) {
            if (e.from < e.to) {
                edges.push_back(e);
            }
        }
    }
    
    //auto cmp = [](Edge& a, Edge& b) {return a.weight > b.weight; };
    //std::make_heap(edges.begin(), edges.end(), cmp);

    std::ranges::make_heap(edges, std::greater<Edge>{});

    DSets D(size + 1);

    //std::sort_heap(edges.begin(), edges.end());

    int t = 0;

    int counter = 0;

    while (counter < size - 1) {
        
        Edge e = edges.front();
        std::ranges::pop_heap(edges, std::greater<Edge>{});
        edges.pop_back();
        
        // C:/Skola/TND004 - Datastrukturer/lab4/code/code4b/graph1.txt

        if (D.find(e.to) != D.find(e.from)) {
            D.join(D.find(e.to), D.find(e.from));
            std::cout << e.from << " -- " << e.to << " [weight: " << e.weight << "]\n";
            counter++;
            t += e.weight;
        }


    }

    D.print();

    std::cout << "total weight: " << t << "\n";


}

// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
