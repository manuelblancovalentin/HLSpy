//
// Created by Manuel Blanco Valentin on 1/10/21.
//

#include "hierarchy.h"
#include <iostream>
#include <fstream>
#include "gui.h"

// Save to json method
int Hierarchy::save_json(std::string FILEPATH) {

    // Open stream
    std::ofstream file_id;
    file_id.open(FILEPATH);

    // Create json writer to file and write
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(
            builder.newStreamWriter());
    writer->write(Hierarchy::__hierarchy__, &file_id);

    // Close file
    file_id.close();

    // Print to console
    std::cout << "[INFO] - Hierarchy successfully saved to file " + FILEPATH << std::endl;

    return 0;
}

// Print tree method
int Hierarchy::__print__(Json::Value& h, std::string FILEPATH = ""){

    // Get tree
    std::string s = Hierarchy::__tree__;

    // Compile if empty
    if (s.empty()){
        // Call tree function on hierarchy
        s = tree(h, "", true, true);

        // Store tree in Hierarchy object
        Hierarchy::__tree__ = s;
    }

    // Print to console if filename is empty
    if (FILEPATH.empty()){
        std::cout << std::endl << s << std::endl;
    } else {
        std::ofstream of;
        of.open (FILEPATH);
        of << s;
        of.close();

        // Print to console
        std::cout << "[INFO] - Tree successfully saved to file " + FILEPATH << std::endl;

    }


    return 0;
}