//
// Created by esha-naveed on 3/30/26.
//
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

template <class T>
void Edmonds_karp(Graph<T> *g, T source, T sink) {
    Vertex<T> *s = g->getVertex(source);
    Vertex<T> *t = g->getVertex(sink);

    //in case parser fails, findVertex will return nullptr thuis checking
    if (s==nullptr || t==nullptr) return;

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
            auto e = v->setPath();
            //adding flow to the forward edge
            e->setFlow(e->getFlow() + n_min);
            //setting flow to -> the initial flow at the edge+cureent min path

            //minus flow from the prev edge
            auto rev = e->getReverse();
            if (rev!=nullptr) rev->setFlow(rev->getFlow() - n_min);
            //setting flow to-> the prev flow-min path

            v=e->getOrig();
        }
    }
}
