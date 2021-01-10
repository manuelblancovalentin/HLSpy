//
// Created by Manuel Blanco Valentin on 1/9/21.
//

#ifndef HLSPY_DESIGN_H
#define HLSPY_DESIGN_H

#include <string>
#include <vector>
#include "blocks.h"
#include "../jsoncpp/json/json.h"
#include "../helpers/hierarchy.h"

// Design class
class Design {

private:
    Json::Value __recursive_seeker__(VerilogBlock& vblock, std::vector<VerilogBlock>& mod_definitions);
    Hierarchy __hierarchy__;

public:
    std::string NAME = "design";
    std::vector <std::string> SOURCES;
    std::vector <std::string> LIB;
    std::vector <VerilogBlock> ModuleDefinitions;

    // Get hierarchy definition
    Hierarchy get_hierarchy(void);

    // Constructor
    Design(std::string Name, std::vector<std::string> Sources, std::vector<std::string> Lib = {}){
        NAME = Name;
        SOURCES = Sources;
        LIB = Lib;
    }
};


#endif //HLSPY_DESIGN_H
