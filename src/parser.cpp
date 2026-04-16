#include "parser.h"

#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

void parser(const string& filename, projectData& data) {
    //opeingin file
    ifstream file(filename);
    //incase not found
    if (!file.is_open()) { printf("Cannot find file"); }

    string line;
    int mode=0;
    /*
     *mode = 1 -> submission
     *mode = 2 -> reviewers
     *mode = 3 -> parameters
     *mode = 4 -> control
     */
    while (getline(file, line)) {
        if (line.empty()) { continue; }

        //setting the mode
        if (line[0] == '#') {
            //if (whatever to find != not found) 77causing unnecessary errors
            if (line.find("#Submissions")!= std::string::npos) mode=1;
            else if (line.find("#Reviewers")!= std::string::npos) mode=2;
            else if (line.find("#Parameters")!= std::string::npos) mode=3;
            else if (line.find("#Control")!= std::string::npos) mode=4;
            continue;
        }

        //adding data
        stringstream ss(line);
        string item;
        vector<std::string> row;
        //dealing with csv data
        while (getline(ss, item, ',')) { row.push_back(item); }
        //if empty line - move on!
        if (row.empty() || row[0].empty()) continue;

        try {
            //checking row.size() to make sure there is stuff there
            if (mode == 1 && row.size() >= 5) {
                Submission s;
                s.id = stoi(row[0]);
                s.primary = stoi(row[4]);
                if (row.size() > 5 && !row[5].empty() && row[5] != std::string(" ")) {
                    s.secondary = stoi(row[5]);
                } else {
                    s.secondary = 0;
                }
                data.submissions.push_back(s);
                data.nodeToRealID[data.submissions.size()] = s.id;
            }
            else if (mode == 2 && row.size() >= 4) {
                Reviewers r;
                r.id = stoi(row[0]);
                r.primary = stoi(row[3]);
                if (row.size() > 4 && !row[4].empty() && row[4] != std::string(" ")) {
                    r.secondary = stoi(row[4]);
                } else {
                    r.secondary = 0;
                }
                data.reviewers.push_back(r);
                int revNodeIdx = data.submissions.size() + data.reviewers.size();
                data.nodeToRealID[revNodeIdx] = r.id;
            }
            else if (mode == 3 || mode == 4) {
                if (row.size() < 2) continue;
                if (row[0] == "MinReviewsPerSubmission") data.config.minReviewsPerSubmission = stoi(row[1]);
                if (row[0] == "MaxReviewsPerReviewer") data.config.maxReviewsPerReviewer = stoi(row[1]);
                if (row[0] == "PrimaryReviewerExpertise") data.config.primaryReviewerExpertise = stoi(row[1]);
                if (row[0] == "SecondaryReviewerExpertise") data.config.secondaryReviewerExpertise = stoi(row[1]);
                if (row[0] == "PrimarySubmissionDomain") data.config.primarySubmissionDomain = stoi(row[1]);
                if (row[0] == "SecondarySubmissionDomain") data.config.secondarySubmissionDomain = stoi(row[1]);
                if (row[0] == "GenerateAssignments") data.config.generateAssignments = stoi(row[1]);
                if (row[0] == "RiskAnalysis") data.config.riskAnalysis = stoi(row[1]);
                if (row[0] == "OutputFileName") data.config.outputFileName = string(row[1]);
            }
        } catch (const exception& e) {
            cout << " Error" << endl;
        }
    }
}