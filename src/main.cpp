/**
* @file main.cpp
 * @brief Entry point for the Graph-based Assignment System.
 * @details Provides both an interactive menu-driven interface and a non-interactive
 * batch mode for processing paper-reviewer assignments using max-flow algorithms.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "dataStructures.h"
#include "parser.h"
#include "createGraph.h"
#include "graph.h"
#include "algorithm.h"

using namespace std;

/**
 * @brief Executes the assignment pipeline without user intervention.
 * @details Parses the input, builds the flow network, runs Edmonds-Karp,
 * and exports the results directly to a file.
 * @param inputFile Path to the source CSV dataset.
 * @param outputFile Path where the results should be saved.
 * @param data Reference to the projectData structure to store parsed information.
 */
void runBatchMode(string inputFile, string outputFile, projectData& data) {
    //1.
    parser(inputFile, data);
    if (data.submissions.empty()) {
        cerr << "[Error] Batch Mode: could not parse input file, or file is empty" << endl;
        return;
    }

    //2.
    Graph<int> flowGraph = createGraph::buildGraph(data);
    int S = 0;
    int T = data.submissions.size() + data.reviewers.size() + 1;

    //3.
    Edmonds_karp(&flowGraph, S, T);

    //Output results
    /*
    ofstream out(outputFile);
    if (!out.is_open()) {
        cerr << "[Error] Batch Mode: Could not open output file: " << outputFile << endl;
        return;
    }


    out << "--- Assignment Results (Batch Mode) ---" << endl;
    int matches = 0;
    for (int i = 1; i <= data.submissions.size(); ++i) {
        Vertex<int>* v = flowGraph.findVertex(i);
        if (!v) continue;
        for (auto e : v->getAdj()) {
            // Check for forward flow from Submission to Reviewer
            if (e->getFlow() > 0 && e->getDest()->getInfo() != S) {
                out << "Submission " << data.nodeToRealID[i]
                    << " -> Reviewer " << data.nodeToRealID[e->getDest()->getInfo()] << endl;
                matches++;
            }
        }
    }
    out << "Total Assignments: " << matches << endl;
    out.close();*/
    data.config.outputFileName = outputFile;
    createGraph::output_file(data, flowGraph);

    cout << "[Success] Batch execution complete. Results saved to " << outputFile << endl;

}

void displayMenu() {
    cout << "--------------------------" << endl;
    cout << "        Project 1         " << endl;
    cout << "--------------------------" << endl;
    cout << "1. Load Dataset (.csv)" << endl;
    cout << "2. Build Flow Network" << endl;
    cout << "3. Run Edmonds-Karp & Show Assignments" << endl;
    cout << "0. Exit" << endl;
    cout << "--------------------------" << endl;
}

/**
 * @brief Main execution loop.
 * @details Handles command-line arguments for batch mode or launches the
 * interactive terminal menu.
 * * Usage (Batch Mode):
 * @code
 * ./program -b input.csv output.csv
 * @endcode
 * * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Execution status (0 for success).
 */
int main(int argc, char* argv[]) {
    projectData data;

    // --- BATCH MODE CHECK ---
    // Syntax: myProg -b input.csv output.csv
    if (argc >= 4 && string(argv[1]) == "-b") {
        int files = argc;
        string outputFile = argv[files-1];
        for (int i=2; i<argc-1; i++) {
            string inputFile = argv[i];
            cout << "[Batch] Processing: " << inputFile << " -> " << outputFile << endl;
            data.submissions.clear();
            data.reviewers.clear();
            data.nodeToRealID.clear();
            runBatchMode(inputFile, outputFile, data);
        }
        return 0; // Exit after batch processing
    }

    // --- INTERACTIVE MODE ---
    bool dataLoaded = false;
    bool graphBuilt = false;
    Graph<int> flowGraph;

    int choice = -1;
    while (choice != 0) {
        displayMenu();

        if (!(cin >> choice)) {
            cout << "[Error] Please enter a valid numerical choice." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                string defaultPath = "../dataset/input/dataset1.csv";
                char useDefault;

                cout << "Use default dataset? (y/n): ";
                cin >> useDefault;

                string finalPath;
                if (useDefault == 'y' || useDefault == 'Y') {
                    finalPath = defaultPath;
                } else {
                    cout << "Enter full path: ";
                    cin >> finalPath; // Simple cin works here because we aren't expecting spaces
                }

                cout << "[System] Loading: " << finalPath << endl;
                parser(finalPath, data);

                if (data.submissions.empty()) {
                    cout << "[Error] Failed to load data." << endl;
                } else {
                    cout << "[Success] Loaded " << data.submissions.size() << " entries." << endl;
                    dataLoaded = true;
                }
                break;
            }
            case 2: {
                if (!dataLoaded) {
                    cout << "[Error] Load data first (Option 1)." << endl;
                    break;
                }
                flowGraph = createGraph::buildGraph(data);
                graphBuilt = true;
                cout << "[Success] Network constructed." << endl;
                break;
            }
                /*
            case 3: {
                /*
                if (!graphBuilt) {
                    cout << "[Error] Build graph first (Option 2)." << endl;
                    break;
                }
                int S = 0;
                int T = data.submissions.size() + data.reviewers.size() + 1;

                int matches = Edmonds_karp(&flowGraph, S, T);

                cout << "\n--- ASSIGNMENT RESULTS ---" << endl;
                for (int i = 1; i <= data.submissions.size(); ++i) {
                    Vertex<int>* v = flowGraph.findVertex(i);
                    if (!v) continue;
                    for (auto e : v->getAdj()) {
                        if (e->getFlow() > 0 && e->getDest()->getInfo() > (int)data.submissions.size() && e->getDest()->getInfo() <= (int)(data.submissions.size() + data.reviewers.size())) {
                            cout << "Sub " << data.nodeToRealID[i] << " -> Rev " << data.nodeToRealID[e->getDest()->getInfo()] << endl;
                           // Essential for Task 1.1:
                           cout << "   [Path: 0 -> " << i << " -> " << e->getDest()->getInfo() << " -> " << T << "]" << endl;
                        }
                    }
                }
                cout << "\n[Success] Total Assignments: " << matches << endl;
                cout << "[Success] Results exported to: " << data.config.outputFileName << endl;
                if (data.config.riskAnalysis == 1) {
                    cout << "[Info] Risk Analysis was performed and added to the CSV." << endl;
                }
                break;
                if (!graphBuilt) {
                cout << "[Error] Build graph first (Option 2)." << endl;
                break;
            }
                int S = 0;
                int T = data.submissions.size() + data.reviewers.size() + 1;
                int numSub = data.submissions.size();
                int numRev = data.reviewers.size();

                int totalMatches = Edmonds_karp(&flowGraph, S, T);

                // 1. Print Submission -> Reviewer Assignments
                cout << "#SubmissionId,ReviewerId,Match" << endl;
                for (int i = 1; i <= numSub; i++) {
                    Vertex<int>* v = flowGraph.findVertex(i);
                    if (!v) continue;
                    for (auto e : v->getAdj()) {
                        if (e->getFlow() > 0 && e->getDest()->getInfo() > numSub && e->getDest()->getInfo() <= (numSub + numRev)) {
                            cout << data.nodeToRealID[i] << ", "
                                 << data.nodeToRealID[e->getDest()->getInfo()] << ", "
                                 << data.submissions[i-1].primary << endl;
                        }
                    }
                }

                // 2. Print Reviewer -> Submission Assignments
                cout << "#ReviewerId,SubmissionId,Match" << endl;
                for (int j = 0; j < numRev; j++) {
                    int revNodeID = numSub + j + 1;
                    // We look at all submission nodes to see who sent flow to this reviewer
                    for (int i = 1; i <= numSub; i++) {
                        Vertex<int>* subV = flowGraph.findVertex(i);
                        if (!subV) continue;
                        for (auto e : subV->getAdj()) {
                            if (e->getDest()->getInfo() == revNodeID && e->getFlow() > 0) {
                                cout << data.nodeToRealID[revNodeID] << ", "
                                     << data.nodeToRealID[i] << ", "
                                     << data.reviewers[j].primary << endl;
                            }
                        }
                    }
                }

                // 3. Print Total
                cout << "#Total: " << totalMatches << endl;

                // 4. Print Missing Reviews
                cout << "#SubmissionId,Domain,MissingReviews" << endl;
                for (int i = 1; i <= numSub; i++) {
                    Vertex<int>* v = flowGraph.findVertex(i);
                    int currentFlow = 0;
                    if (v) {
                        for (auto e : v->getAdj()) {
                            if (e->getFlow() > 0 || e->getDest()->getInfo() > numSub || e->getDest()->getInfo() <= (numSub + numRev)) {
                                currentFlow += (int)e->getFlow();
                            }
                        }
                    }
                    int missing = data.config.minReviewsPerSubmission - currentFlow;
                    if (missing > 0) {
                        cout << data.nodeToRealID[i] << ", "
                             << data.submissions[i-1].primary << ", "
                             << missing << endl;
                    }
                }
                break;
            }
            */
                case 3: {
    if (!graphBuilt) {
        cout << "[Error] Build graph first (Option 2)." << endl;
        break;
    }
    int S = 0;
    int T = data.submissions.size() + data.reviewers.size() + 1;
    int numSub = data.submissions.size();
    int numRev = data.reviewers.size();

    int totalMatches = Edmonds_karp(&flowGraph, S, T);

    // 1. Print Submission -> Reviewer Assignments
    cout << "#SubmissionId,ReviewerId,Match" << endl;
    for (int i = 1; i <= numSub; i++) {
        Vertex<int>* v = flowGraph.findVertex(i);
        if (!v) continue;
        for (auto e : v->getAdj()) {
            // FIX: Use && (AND) to ensure it HAS flow AND is pointing to a Reviewer
            if (e->getFlow() > 0 && e->getDest()->getInfo() > numSub && e->getDest()->getInfo() <= (numSub + numRev)) {
                cout << data.nodeToRealID[i] << ", "
                     << data.nodeToRealID[e->getDest()->getInfo()] << ", "
                     << data.submissions[i-1].primary << endl;
            }
        }
    }

    // 2. Print Reviewer -> Submission Assignments
    cout << "#ReviewerId,SubmissionId,Match" << endl;
    for (int j = 0; j < numRev; j++) {
        int revNodeID = numSub + j + 1;
        for (int i = 1; i <= numSub; i++) {
            Vertex<int>* subV = flowGraph.findVertex(i);
            if (!subV) continue;
            for (auto e : subV->getAdj()) {
                // Check if this specific submission node sent flow to the current reviewer
                if (e->getDest()->getInfo() == revNodeID && e->getFlow() > 0) {
                    cout << data.nodeToRealID[revNodeID] << ", "
                         << data.nodeToRealID[i] << ", "
                         << data.reviewers[j].primary << endl;
                }
            }
        }
    }

    // 3. Print Total
    cout << "#Total: " << totalMatches << endl;

    // 4. Print Missing Reviews
    cout << "#SubmissionId,Domain,MissingReviews" << endl;
    for (int i = 1; i <= numSub; i++) {
        Vertex<int>* v = flowGraph.findVertex(i);
        int currentFlow = 0;
        if (v) {
            for (auto e : v->getAdj()) {
                // FIX: Use && (AND) to count only actual assigned flows
                if (e->getFlow() > 0 && e->getDest()->getInfo() > numSub && e->getDest()->getInfo() <= (numSub + numRev)) {
                    currentFlow += (int)e->getFlow();
                }
            }
        }
        int missing = data.config.minReviewsPerSubmission - currentFlow;
        if (missing > 0) {
            cout << data.nodeToRealID[i] << ", "
                 << data.submissions[i-1].primary << ", "
                 << missing << endl;
        }
    }
    break;
}
            case 0:
                cout << "Exiting application." << endl;
                break;
            default:
                cout << "[Warning] Invalid selection." << endl;
        }
    }
    return 0;
}