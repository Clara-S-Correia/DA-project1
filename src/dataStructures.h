#ifndef DA_PROJECT1_DATASTRUCTURES_H
#define DA_PROJECT1_DATASTRUCTURES_H

#include <map>
#include <string>
#include <vector>

struct Submission {
    int id;
    std::string title;
    std::string authors;
    std::string email;
    std::string primary;
    std::string secondary; //can be empty
};

struct Reviewers {
    int id;
    std::string email;
    std::string primary;
    std::string secondary;
};

//global configuration, from #parameters and #control
struct projectConfig {
    int minReviewsPerSubmission = 0;
    //added the rest of parameters here! :)
    int maxReviewsPerReviewer = 0;
    //values either 1 or 0
    int primaryReviewerExpertise = 0;
    int secondaryReviewerExpertise = 0;
    int primarySubmissionDomain = 0;
    int secondarySubmissionDomain = 0;
    int generateAssignments = 0; //mode 1, 2 or 3
    int riskAnalysis = 0; //0 or 1
    std::string outputFileName;
};

struct projectData {
    std::vector<Submission> submissions;
    std::vector<Reviewers> reviewers;
    projectConfig config;
    std::map<int, int> nodeToRealID;
    /*two map<int, vector<int>>:
    sub-rev
    rev-sub
    //map for either one will have id/key as int and the vector will have the data (that can be multiple revs or subs)
    */
};

#endif //DA_PROJECT1_DATASTRUCTURES_H