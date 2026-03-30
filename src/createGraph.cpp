//
// Created by esha-naveed on 3/30/26.
//

#include "graph.h"
#include "dataStructures.h"
#include "createGraph.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

Graph<int> createGraph::buildGraph(projectData &data) {
    //basic info
    Graph<int> g;
    int numSub = data.submissions.size();
    int numRev = data.reviewers.size();

    //S(super source), T(super sink),
    int S = 0;
    int T = numSub + numRev + 1;

    //adding all vertices to g
    for (int i = 0; i <= T; i++) { g.addVertex(i); }

    //grapgh: S->subs->revs->T
    /*
    *vertex 0: super source s
    vertex 1 to n: submissions
    vertex n+1 to n+m: reviewers
    vertex n+m+1: super-sink t
    */

    //S->subs
    //capacity==MinReviewsPerSubmission
    for (int i = 0; i < numSub; i++) {
        int nodeID = i + 1;
        data.nodeToRealID[nodeID] = data.submissions[i].id;
        g.addFlowEdge(S, nodeID, data.config.minReviewsPerSubmission);
    }

    //revs->T
    //capacity==MaxReviewsPerReviewer
    for (int j = 0; j < numRev; j++) {
        int nodeID = numSub + j + 1;
        data.nodeToRealID[nodeID] = data.reviewers[j].id;
        g.addFlowEdge(nodeID, T, data.config.maxReviewsPerReviewer);
    }

    //based on mode, connect subs->revs
    for (int i = 0; i < numSub; i++) {
        for (int j = 0; j < numRev; j++) {
            const auto& sub = data.submissions[i];
            const auto& rev = data.reviewers[j];
            bool match = false;

            //based on GenerateAssignments
            if (data.config.generateAssignments == 1) {
                //1: matches primary subs to primary revs
                if (sub.primary == rev.primary) match = true;
            }
            else if (data.config.generateAssignments == 2) {
                //2: only primary revs can be linked to subs, first to primary subs then secondary
                if ((sub.primary == rev.primary || sub.secondary == rev.primary)) match = true;
            }
            else if (data.config.generateAssignments == 3) {
                //3: either rev to either sub with parameters from the 4 other things
                //checking for the 4 parameters:
                bool pSub = (data.config.primarySubmissionDomain == 1);
                bool sSub = (data.config.secondarySubmissionDomain == 1);
                bool pRev = (data.config.primaryReviewerExpertise== 1);
                bool sRev = (data.config.secondaryReviewerExpertise == 1);

                //checking the conditions + the id thingy for each sub and rev
                if (pSub && pRev && sub.primary == rev.primary) match = true;
                if (sSub && pRev && sub.secondary == rev.primary) match = true;
                if (pSub && sRev && sub.primary == rev.secondary) match = true;
                if (sSub && sRev && sub.secondary == rev.secondary) match = true;
            }

            if (match) {
                //capacity:paper to review - 1, one paper goes to one reviewer, one rev can take many subs
                g.addFlowEdge(i + 1, numSub + j + 1, 1);


            }
        }
    }
    return g;
}
