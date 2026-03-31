/**
* @file algorithm.h
 * @brief Implementation of the Edmonds-Karp algorithm for Maximum Flow.
 * @details This file contains the logic for finding augmenting paths via BFS
 * and updating the residual graph to compute the optimal matching between
 * submissions and reviewers.
 */

#ifndef DA_PROJECT1_ALGORITHM_H
#define DA_PROJECT1_ALGORITHM_H


#include "graph.h"
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;
/*
 * Purpose: matching submission to reviewers based on expertise(primary/secondary)
 * BFS_part: searches the graph to find the path from start(source S) to the end(sink T)
 * rules: path is only made with routes that still have capacity
 * if a path is not found - means that max num of reviews assigned
 *
 * Edmonds-karp: once a path is found by BFS_path, it calculates the n_min-> also called Bottleneck
 * n_min is the smallest space left
 * it adds the flow to every path and also unpadtes thr reverse paths
 * in case a diffenret match needs to be made
 */

#define INF std::numeric_limits<double>::max()
/**
 * @brief Finds an augmenting path in the residual graph using Breadth-First Search.
 * @details Explores the graph starting from the source to find the shortest path (by number of edges)
 * to the sink that still has available residual capacity (Capacity - Flow > 0).
 * * @tparam T The type of data stored in the vertices (typically int).
 * @param g Pointer to the Graph object.
 * @param s Pointer to the source Vertex.
 * @param t Pointer to the sink Vertex.
 * @return true If a path to the sink was found.
 * @return false If no more augmenting paths exist.
 * * @time_complexity O(V + E) where V is the number of vertices and E is the number of edges.
 */

//infinity - copied from graph.h. is the max possible value
//so any capacity found is smaller than this

//capacity - size of the pipe
/*
 * from S to subs -> cap is no. of reviews a sub needs
 * rev to T -> cap is max reviews by each rev
 * sub to rev -> 1, since same sub cannot be reviewed by same rev
 */
//flow - amount passing through
//base structure for this in WS3-Ex5
template <class T>
bool BFS_part(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
    // Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    // Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<T> *> q;
    q.push(s);

    // BFS to find an augmenting path
    while(!q.empty()) {
        auto v = q.front();
        q.pop();

        for(auto e: v->getAdj()) {
            auto w = e->getDest();
            //residual capacity: Capacity-Flow > 0
            auto residual = e->getCapacity()- e->getFlow();
            if (w != nullptr && !w->isVisited() && residual>0) {
                w->setVisited(true);
                w->setPath(e);
                q.push(w);
                //if path is made to the sink(T)
                if (w == t) return true;
            }
        }
    }

    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

/**
 * @brief Computes the Maximum Flow using the Edmonds-Karp algorithm.
 * @details Iteratively calls BFS_part to find augmenting paths. For each path, it identifies
 * the "bottleneck" capacity and updates the flow along the forward and reverse edges.
 * * @tparam T The type of data stored in the vertices.
 * @param g Pointer to the Graph object.
 * @param source The identifier for the source node (usually 0).
 * @param sink The identifier for the sink node (usually numSub + numRev + 1).
 * * @time_complexity O(V * E^2). In a flow network, this ensures the algorithm terminates
 * in polynomial time regardless of the capacity values.
 * @space_complexity O(V) for the queue and path tracking used in BFS.
 */
template <class T>
int Edmonds_karp(Graph<T> *g, T source, T sink) {
    Vertex<T> *s = g->findVertex(source);
    Vertex<T> *t = g->findVertex(sink);

    //in case parser fails, findVertex will return nullptr thuis checking
    if (s==nullptr || t==nullptr) return 0;

    int total_flow = 0;

    //while bfs(graph, source, sink)
    while (BFS_part(g, s, t)) {
        double n_min = INF;

        //looking for n_max capacity of path by bfs
        for (auto v=t ; v!=s; ) {
            //starting from sink, until the source
            auto e = v->getPath();//get edge of vertex v
            n_min = min(n_min, e->getCapacity()-e->getFlow());//getting smaller one, the current max or the path found
            v=e->getOrig();//moving backwards - getting the original
        }

        for (auto v=t ; v!=s; ) {
            auto e = v->getPath();
            //adding flow to the forward edge
            e->setFlow(e->getFlow() + n_min);
            //setting flow to -> the initial flow at the edge+cureent min path

            //minus flow from the prev edge
            auto rev = e->getReverse();
            if (rev!=nullptr) rev->setFlow(rev->getFlow() - n_min);
            //setting flow to-> the prev flow-min path

            v=e->getOrig();
        }
        total_flow += n_min;
    }
    return total_flow;
}


#endif //DA_PROJECT1_ALGORITHM_H