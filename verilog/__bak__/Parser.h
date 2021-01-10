//
// Created by Manuel Blanco Valentin on 1/6/21.
//

#ifndef HLSPY_PARSER_H
#define HLSPY_PARSER_H

#include <string>
#include "../helpers/gui.h"
#include "../helpers/basic.h"
#include "blocks.h"

// Parser class
class Parser {
private:

    // __parse__ function
    VerilogBlock __parse__(std::vector <VerilogBlock>& module_definitions,
                           std::vector <std::string>& sources,
                           std::vector <std::string>& lib,
                           std::string FILENAME = "",
                           std::string TEXT = "",
                           int nlines = -1,
                           int line_offset = 0,
                           std::string TAB = "",
                           std::string NAME = "block");

    // Function to count number lines
    int __count_number_lines__(const char *FILENAME);

public:
    // Inputs for initializer
    std::string FILENAME = "";
    std::string TEXT = "";
    std::string TAB = "";
    std::string NAME = "block";

    // Initialize progress Bar that will be used later while parsing
    progressBar pbar;

    // Number of lines
    int num_lines = -1;

    // Line offset
    int line_offset = 0;

    // Initialization
    Parser(std::string FILENAME = "", std::string TEXT = "",
           progressBar pbar = progressBar(), int num_lines = -1,
           int line_offset = 0, std::string TAB = "",
           std::string NAME = "block"): FILENAME(FILENAME), TEXT(TEXT), pbar(pbar), num_lines(num_lines),
           line_offset(line_offset), TAB(TAB), NAME(NAME) {}
    /*  The above use of Initializer list is optional as the
        constructor can also be written as:
        Parser(const char *F) {
            FILENAME = F;
        }
    */

    // Functionalities
    VerilogBlock parse(std::vector<VerilogBlock> &module_definitions,
                       std::vector <std::string>& sources,
                       std::vector <std::string>& lib) {return __parse__(module_definitions, sources, lib,
                                                                             FILENAME, TEXT, num_lines,
                                                                             line_offset, TAB, NAME);}

};

bool LineChecker(std::string mainStr, std::vector<std::string> patterns, int n_patterns);


#endif //HLSPY_PARSER_H
