#include "parser.h"

#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

void parser(const std::string& filename, projectData& data) {
    //opeingin file
    std::ifstream file(filename);
    //incase not found
    if (!file.is_open()) { printf("Cannot find file"); }

    std::string line;
    int mode=0;
    /*
     *mode = 1 -> submission
     *mode = 2 -> reviewers
     *mode = 3 -> parameters
     *mode = 4 -> control
     */
    while (std::getline(file, line)) {
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
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;
        //dealing with csv data
        while (std::getline(ss, item, ',')) { row.push_back(item); }
        //if empty line - move on!
        if (row.empty() || row[0].empty()) continue;

        try {
            //checking row.size() to make sure there is stuff there
            if (mode == 1 && row.size() >= 2) {
                Submission s;
                s.id = std::stoi(row[0]);
                s.title = row[1];
                data.submissions.push_back(s);
            }
            else if (mode == 2 && row.size() >= 1) {
                Reviewers r;
                r.id = std::stoi(row[0]);
                data.reviewers.push_back(r);
            }
            else if (mode == 3 || mode == 4) {
                if (row.size() < 2) continue;
                if (row[0] == "MinReviewsPerSubmission") data.config.minReviewsPerSubmission = std::stoi(row[1]);
                if (row[0] == "MaxReviewsPerReviewer") data.config.maxReviewsPerReviewer = std::stoi(row[1]);
                if (row[0] == "PrimaryReviewerExpertise") data.config.primaryReviewerExpertises = std::stoi(row[1]);
                if (row[0] == "SecondaryReviewerExpertise") data.config.secondaryReviewerExpertise = std::stoi(row[1]);
                if (row[0] == "PrimarySubmissionDomain") data.config.primarySubmissionDomain = std::stoi(row[1]);
                if (row[0] == "SecondarySubmissionDomain") data.config.secondarySubmissionDomain = std::stoi(row[1]);
                if (row[0] == "GenerateAssignments") data.config.generateAssignments = std::stoi(row[1]);
                if (row[0] == "RiskAnalysis") data.config.riskAnalysis = std::stoi(row[1]);
                if (row[0] == "OutputFileName") data.config.outputFileName = std::string(row[1]);
            }
        } catch (const std::exception& e) {
            std::cout << " Error" << std::endl;
        }

    }

}

