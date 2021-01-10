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
                           std::ifstream& stream,
                           progressBar& pbar,
                           int& start_line,
                           std::string previous_line = "",
                           std::string TAB = "",
                           std::string NAME = "block",
                           std::string REF = "");

    VerilogBlock __parse_file__(std::vector <VerilogBlock>& module_definitions,
                                std::vector <std::string>& sources,
                                std::vector <std::string>& lib,
                                std::string FILENAME,
                                std::string NAME);

    // Function to count number lines
    int __count_number_lines__(const char *FILENAME);

public:
    // Inputs for initializer
    std::string FILENAME = "";
    std::string TAB = "";
    std::string NAME = "block";


    // Number of lines
    int num_lines = -1;

    // Line offset
    int line_offset = 0;

    // Initialization
    Parser(std::string FILENAME = "", std::string NAME = "block"): FILENAME(FILENAME), NAME(NAME) {}

    // Functionalities
    VerilogBlock parse(std::vector<VerilogBlock> &module_definitions,
                       std::vector <std::string>& sources,
                       std::vector <std::string>& lib) {return __parse_file__(module_definitions,
                                                                              sources,
                                                                              lib,
                                                                              FILENAME,
                                                                              NAME);}

};

bool LineChecker(std::string mainStr, std::vector<std::string> patterns, int n_patterns);


#endif //HLSPY_PARSER_H
