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

void runBatchMode(string inputFile, string outputFile, projectData& data) {
    //1.
    parser(inputFile, data);
    if (data.submissions.empty()) {
        cerr << "[Error] BAtch Mode: could not parse input file, or file is empty" << endl;
        return;
    }

    //2.
    Graph<int> flowGraph = createGraph::buildGraph(data);
    int S = 0;
    int T = data.submissions.size() + data.reviewers.size() + 1;

    //3.
    Edmonds_karp(&flowGraph, S, T);

    //Output results
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
    out.close();

    cout << "[Success] Batch execution complete. " << matches << " assignments saved to " << outputFile << endl;
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

int main(int argc, char* argv[]) {
    projectData data;
    // --- BATCH MODE CHECK ---
    // Syntax: myProg -b input.csv output.csv
    if (argc >= 4 && string(argv[1]) == "-b") {
        string inputFile = argv[2];
        string outputFile = argv[3];
        runBatchMode(inputFile, outputFile, data);
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
            case 3: {
                if (!graphBuilt) {
                    cout << "[Error] Build graph first (Option 2)." << endl;
                    break;
                }
                int S = 0;
                int T = data.submissions.size() + data.reviewers.size() + 1;

                Edmonds_karp(&flowGraph, S, T);

                cout << "\n--- ASSIGNMENT RESULTS ---" << endl;
                for (int i = 1; i <= data.submissions.size(); ++i) {
                    Vertex<int>* v = flowGraph.findVertex(i);
                    if (!v) continue;
                    for (auto e : v->getAdj()) {
                        if (e->getFlow() > 0 && e->getDest()->getInfo() != S) {
                            cout << "Sub " << data.nodeToRealID[i] << " -> Rev "
                                 << data.nodeToRealID[e->getDest()->getInfo()] << endl;
                            // Showing path details as per Task 1.1 requirement
                            cout << "   [Path: 0 -> " << i << " -> " << e->getDest()->getInfo() << " -> " << T << "]" << endl;
                        }
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