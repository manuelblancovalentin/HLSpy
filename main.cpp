// Basic libs
#include <iostream>

// Utilities
#include "./verilog/Design.h"
#include "./helpers/hierarchy.h"

int main() {

    int test = 0;
    std::string NAME;
    std::vector <std::string> SOURCES = {};
    std::vector <std::string> LIB = {};
    std::string OUTFILE, OUTFILE_TXT;

    switch (test){
        case 0:
            NAME = "ECON_AI";
            SOURCES = {"/Users/mbvalentin/CLionProjects/HLSpy/examples/ECON_AE/auto_encoder_wrap_sim.v",
                       "/Users/mbvalentin/CLionProjects/HLSpy/examples/ECON_AE/auto_encoder_wrap_tb.v"};
            OUTFILE = "/Users/mbvalentin/CLionProjects/HLSpy/examples/ECON_AE/ECON_AI.json";
            OUTFILE_TXT = "/Users/mbvalentin/CLionProjects/HLSpy/examples/ECON_AE/ECON_AI.txt";
            break;

        case 1:
            NAME = "COUNTER";
            SOURCES = {"/Users/mbvalentin/CLionProjects/HLSPy/examples/counter/tb.v"};
            LIB = {"/Users/mbvalentin/CLionProjects/HLSPy/examples/counter/"};
            OUTFILE = "/Users/mbvalentin/CLionProjects/HLSPy/examples/counter/hierarchy.json";
            OUTFILE_TXT = "/Users/mbvalentin/CLionProjects/HLSPy/examples/counter/hierarchy.txt";

    };


    // Init Design
    Design d(NAME, SOURCES, LIB);
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
