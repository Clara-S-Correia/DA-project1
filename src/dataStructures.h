/**
* @file dataStructures.h
 * @brief Global data structures for the project.
 * @details Defines the core entities (Submissions, Reviewers) and the
 * configuration settings used across the project.
 */

#ifndef DA_PROJECT1_DATASTRUCTURES_H
#define DA_PROJECT1_DATASTRUCTURES_H

#include <map>
#include <string>
#include <vector>

/**
 * @struct Submission
 * @brief Represents a paper submitted to the conference.
 */
struct Submission {
    int id;
    std::string title;
    std::string authors;
    std::string email;
    std::string primary;
    std::string secondary; //can be empty
};

/**
 * @struct Reviewers
 * @brief Represents a conference reviewer and their expertise.
 */
struct Reviewers {
    int id;
    std::string email;
    std::string primary;
    std::string secondary;
};

/**
 * @struct projectConfig
 * @brief Global settings parsed from the #Parameters and #Control sections.
 */
struct projectConfig {
    int minReviewsPerSubmission = 0;
    //added the rest of parameters here! :)
    int maxReviewsPerReviewer = 0;
    //values either 1 or 0
    int primaryReviewerExpertise = 0; /**< Toggle for using primary expertise */
    int secondaryReviewerExpertise = 0; /**< Toggle for using secondary expertise */
    int primarySubmissionDomain = 0; /**< Toggle for using primary domain */
    int secondarySubmissionDomain = 0; /**< Toggle for using secondary domain */
    int generateAssignments = 0; /**< Mode: 1 (Primary), 2 (Pri-Rev), 3 (Full) */
    int riskAnalysis = 0; /**< Boolean: 0 or 1 */
    std::string outputFileName;
};

/**
 * @struct projectData
 * @brief The "Master" container that holds all project state.
 * @details This is passed by reference to the Parser and Graph Creator.
 */
struct projectData {
    std::vector<Submission> submissions;
    std::vector<Reviewers> reviewers;
    projectConfig config;

    /**
     * @brief Maps internal Graph node IDs back to real Submission/Reviewer IDs.
     * @details Crucial for: translating the Max-Flow result into a readable format.
     */
    std::map<int, int> nodeToRealID;
    /*two map<int, vector<int>>:
    sub-rev
    rev-sub
    //map for either one will have id/key as int and the vector will have the data (that can be multiple revs or subs)
    */
};

#endif //DA_PROJECT1_DATASTRUCTURES_H