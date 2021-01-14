// Basic libs
#include <iostream>

// Utilities
#include "./verilog/design.h"
#include "helpers/flags.h"

int main() {

    // Setup flags
    FLAGS flags(FLAGS::NONE);
    flags |= FLAGS::AUTO_INCLUDE;

    int test = 0;
    std::string NAME;
    std::vector <std::string> SOURCES = {};
    std::vector <std::string> LIB = {};
    std::string OUTFILE, OUTFILE_TXT;

    switch (test){
        case 0:
            NAME = "ECON_AI";
            SOURCES = {"/Users/mbvalentin/CLionProjects/HackDL/examples/ECON_AE/auto_encoder_wrap_sim.v",
                       "/Users/mbvalentin/CLionProjects/HackDL/examples/ECON_AE/auto_encoder_wrap_tb.v"};
            OUTFILE = "/Users/mbvalentin/CLionProjects/HackDL/examples/ECON_AE/ECON_AI.json";
            OUTFILE_TXT = "/Users/mbvalentin/CLionProjects/HackDL/examples/ECON_AE/ECON_AI.txt";
            break;

        case 1:
            NAME = "COUNTER";
            SOURCES = {"/Users/mbvalentin/CLionProjects/HackDL/examples/counter/tb.v"};
            LIB = {"/Users/mbvalentin/CLionProjects/HackDL/examples/counter/"};
            OUTFILE = "/Users/mbvalentin/CLionProjects/HackDL/examples/counter/hierarchy.json";
            OUTFILE_TXT = "/Users/mbvalentin/CLionProjects/HackDL/examples/counter/hierarchy.txt";
            break;
    };


    // Init Design
    Design d(NAME, SOURCES, LIB, &flags);
    // Get hierarchy
    Hierarchy h = d.get_hierarchy();
    // Plot hierarchy to file
    h.save_json(OUTFILE);
    // Print tree to console
    h.print();
    // Print tree to file
    h.print_txt(OUTFILE_TXT);

    return 0;
}
