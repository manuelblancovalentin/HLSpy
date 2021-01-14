//
// Created by Manuel Blanco Valentin on 1/10/21.
//

#ifndef HLSPY_HIERARCHY_H
#define HLSPY_HIERARCHY_H

#include "../jsoncpp/json/json.h"
#include "gui.h"

class Hierarchy {
private:
    //Json::Value __hierarchy__;
    std::string __hierarchy__;
    int __print__(std::string& h, std::string FILEPATH);

public:

    // Tree
    std::string __tree__;

    // Method to save to json
    int save_json(std::string FILEPATH);

    // Method to print tree
    int print(void) {return __print__(__hierarchy__, "");}
    int print_txt(std::string FILEPATH) {return __print__(__hierarchy__,FILEPATH);}

    // Constructor
    Hierarchy(std::string h = ""): __hierarchy__(h) {};
};


#endif //HLSPY_HIERARCHY_H
