#ifndef DA_PROJECT1_DATASTRUCTURES_H
#define DA_PROJECT1_DATASTRUCTURES_H

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
    int maxReviewsPerSubmission = 0;
    int generateAssignments = 0; //mode 1, 2 or 3
    int riskAnalysis = 0; //0 or 1
    std::string outputFileName;
};

struct projectData {
    std::vector<Submission> submissions;
    std::vector<Reviewers> reviewers;
    projectConfig config;
};

#endif //DA_PROJECT1_DATASTRUCTURES_H