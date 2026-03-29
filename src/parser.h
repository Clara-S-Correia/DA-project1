#ifndef DA_PROJECT1_PARSER_H
#define DA_PROJECT1_PARSER_H

#include "dataStructures.h"//to use the sub and rev

//data structure for reading data in general
typedef struct {
    int id;
    int primary;
    int secondary;
} filedata_t;

void parser(const std::string& filename, projectData& data);

#endif //DA_PROJECT1_PARSER_H