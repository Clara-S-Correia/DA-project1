/**
* @file createGraph.h
 * @brief Header file for the graph construction logic.
 * @details This class acts as the bridge between the parsed CSV data and the
 * flow network used for the assignment algorithm.
 */

#ifndef DA_PROJECT1_CREATEGRAPH_H
#define DA_PROJECT1_CREATEGRAPH_H

#include "graph.h"
#include "dataStructures.h"

using namespace std;

/**
 * @class createGraph
 * @brief Provides static methods to transform project data into a flow network.
 */
class createGraph {
public:
    /**
     * @brief Transforms parsed ProjectData into a directed Flow Graph.
     * * This function creates a source node, a sink node, and nodes for all
     * reviewers and submissions. It adds edges with capacities based on
     * expertise matching (Task 2.1).
     * * @param data The projectData structure containing all reviewers and papers.
     * @return A Graph object configured for the Max-Flow algorithm.
     * * @time_complexity O(R * S) where R is the number of reviewers and S is
     * the number of submissions, as we must check matches for every pair.
     * @space_complexity O(V + E) where V = R + S + 2 and E is the number of valid matches.
     */
    static Graph<int> buildGraph(projectData& data);
};

#endif //DA_PROJECT1_CREATEGRAPH_H
