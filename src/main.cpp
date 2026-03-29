#include <iostream>
#include "dataStructures.h"
#include "parser.h"

int main() {
    projectData data;

    parser("/home/esha-naveed/Desktop/DA_PT/DA-project1/dataset/input/dataset2.csv", data);

    std::cout << "checking if data rewading test" << std::endl;
    std::cout << "submissions: " << data.submissions.size() << std::endl;
    for (const auto& s : data.submissions) {
        std::cout << "id: " << s.id << " - title: " << s.title << std::endl;
    }

    std::cout << "revs: " << data.reviewers.size() << std::endl;
    for (const auto& r : data.reviewers) {
        std::cout << "id: " << r.id << std::endl;
    }

    std::cout << "conf:" << std::endl;
    std::cout << "min revs: " << data.config.minReviewsPerSubmission << std::endl;
    std::cout << "max revs: " << data.config.maxReviewsPerSubmission << std::endl;
    std::cout << "gen assignments: " << data.config.generateAssignments << std::endl;

    return 0;
}