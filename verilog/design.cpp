//
// Created by Manuel Blanco Valentin on 1/9/21.
//

#include "design.h"
#include "Parser.h"
#include "blocks.h"
#include <iostream>
#include "../helpers/basic.cpp"
#include "../jsoncpp/json/json.h"
#include "../helpers/hierarchy.h"

// Get hierarchy method
Hierarchy Design::get_hierarchy() {

    // Initialize vlog blocks
    std::vector <VerilogBlock> vblocks;

    // Make sure ModuleDefinitions is empty
    std::vector <VerilogBlock> module_definitions = Design::ModuleDefinitions;

    // We will loop thru all filenames in source
    std::vector <std::string> src_left = Design::SOURCES;

    while (!src_left.empty()){
        // Get first element
        auto& it = src_left[0];
        // Init parser object
        Parser p(it,it);
        // Parse
        VerilogBlock vlogtmp = p.parse(module_definitions, src_left, Design::LIB);
        // Pushback
        vblocks.push_back(vlogtmp);
        // Remove source we just analyzed
        src_left.erase(src_left.begin());

    }

    // Let's try to find the top_module(s) now
    // Fill calls with zeros
    std::vector<int> calls = {};
    for (auto& it: module_definitions) calls.push_back(0);
    bool found_it = false;
    // Create progressBar to display progress for unpatient asses like mine.
    int nmods = calls.size();
    progressBar pbar_top(nmods-1);

    // Now find how many times each module calls other modules
    int nm = 0;
    for (auto& it: module_definitions){
        pbar_top.update(nm,"","");
        nm++;

        int ninst = it.instances.size();
        progressBar pbar_inst(ninst-1);

        int ni = 0;
        for (auto& inst: it.instances){
            pbar_inst.update(ni,"","");
            ni++;
            // Find which module ref this is
            for (int i = 0; i < module_definitions.size(); i++){
                found_it = (std::strcmp(inst.ref.c_str(),module_definitions[i].name.c_str()) == 0);
                if (found_it){
                    calls[i] += 1;
                    break;
                }
            }
        }
    }
    // Now find the module(s) with the least number of calls
    int min_recurrence = INT_MAX;
    for (auto& c: calls){
        if (c < min_recurrence){
            min_recurrence = c;
        }
    }

    std::vector<VerilogBlock*> top_modules;
    for (int i = 0; i < calls.size(); i++){
        if (calls[i] == min_recurrence){
            top_modules.push_back(&(module_definitions[i]));
        }
    }

    // In theory top_modules are all those present in candidates
    // With this, now we can build our design hierarchy. Let's loop thru top_modules.
    std::vector<VerilogBlock*>::iterator citer;
    // Our hierarchy in this case will be a json object, which is extremely powerful
    // and useful for us rn
    Json::Value hierarchy;
    for (int i = 0; i < top_modules.size(); i++){
        // Create entry for this top_module
        VerilogBlock& tmpv = (*top_modules[i]);
        Json::Value tmp = Design::__recursive_seeker__(tmpv, module_definitions);
        // Append to top modules
        std::string h_name = (*top_modules[i]).name;
        hierarchy["top_modules"][h_name] = tmp;
    }

    // Create actual hierarchy object
    Hierarchy h(hierarchy);

    // Set hierarchy in Design (private)
    Design::__hierarchy__ = h;

    // Return and exit
    return h;
}


// Implementation of the recursive seeker that we will use to create our hierarchy
Json::Value Design::__recursive_seeker__(VerilogBlock& vblock,
                                         std::vector<VerilogBlock>& module_definitions) {

    // Init json block
    Json::Value tmp;

    // Check if this is a module or instance
    if (instanceof<VerilogBlock>(&vblock)){

        // Get block name
        std::string name = vblock.name;

        // Get reference
        std::string ref = vblock.ref;

        // Get all fields
        std::vector<Parameter> tmp_params = vblock.parameters;
        Ports tmp_ports = vblock.ports;
        std::vector<NetWire> tmp_netwires = vblock.netwires;
        std::vector<VerilogBlock> tmp_instances = vblock.instances;

        // If the reference does not match the name, it means we have to try to look this up
        // in the module_definitions.
        if (std::strcmp(name.c_str(), ref.c_str()) != 0){
            // Try to find ref in module_definitions
            for (auto& md: module_definitions){
                if (strcmp(md.name.c_str(), ref.c_str()) == 0){
                    // Reassign params
                    tmp_params = md.parameters;
                    // Reassign ports
                    tmp_ports = md.ports;
                    // Reassign netwires
                    tmp_netwires = md.netwires;
                    // Reassign instances
                    tmp_instances = md.instances;
                }
            }
        }

        // Place reference
        tmp["ref"] = ref;

        // Parse parameters
        if (!tmp_params.empty()){
            for (auto& p: tmp_params){
                tmp["parameters"][p.name] = p.value;
            }
        }

        // Ports
        if (!tmp_ports.input.empty() || !tmp_ports.output.empty() || !tmp_ports.inout.empty()){

            // Inputs
            Json::Value tmp_iports(Json::arrayValue);
            for (auto& p: tmp_ports.input){
                tmp_iports.append(p);
            }
            tmp["ports"]["input"] = tmp_iports;

            // Outputs
            Json::Value tmp_oports(Json::arrayValue);
            for (auto& p: tmp_ports.output){
                tmp_oports.append(p);
            }
            tmp["ports"]["output"] = tmp_oports;

            // Inouts
            Json::Value tmp_ioports(Json::arrayValue);
            for (auto& p: tmp_ports.inout){
                tmp_ioports.append(p);
            }
            tmp["ports"]["inout"] = tmp_ioports;
        }

        // Netwires
        for (auto& nw: tmp_netwires){
            // Construct values
            tmp[nw.name]["type"] = nw.type;
            tmp[nw.name]["bitspan"] = nw.bitspan;
            tmp[nw.name]["arrayspan"] = nw.arrayspan;
            tmp[nw.name]["value"] = nw.value;
        }

        // Now the instances
        for (auto& it: tmp_instances){
            // Call recursive seeker on instance
            tmp[it.name] = __recursive_seeker__(it, module_definitions);
        }

    } else {
        std::cout << "What kind of pokémon is this?" << std::endl;
    }

    return tmp;

}