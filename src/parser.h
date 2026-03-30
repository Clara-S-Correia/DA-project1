/**
* @file parser.h
 * @brief Header file for the CSV/Text input parser.
 * @details Responsible for: Reading submissions, reviewers,
 * parameters, and control settings from a formatted file.
 */

#ifndef DA_PROJECT1_PARSER_H
#define DA_PROJECT1_PARSER_H

#include "dataStructures.h"//to use the sub and rev

/**
 * @brief Parses the input file and populates the projectData structure.
 * @details Reads the file line-by-line using a mode-based state machine to
 * distinguish between #Submissions, #Reviewers, #Parameters, and #Control sections.
 * * @param filename The path to the input file (e.g., "input.csv").
 * @param data A reference to the projectData object where parsed info will be stored.
 * * @time_complexity O(L * C) where L is the number of lines in the file and
 * C is the average number of characters per line. This is effectively linear O(N).
 * @space_complexity O(N) where N is the total amount of data stored in memory.
 */
void parser(const std::string& filename, projectData& data);

#endif //DA_PROJECT1_PARSER_H