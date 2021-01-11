//
// Created by Manuel Blanco Valentin on 1/9/21.
//

#include "design.h"
#include "Parser.h"
#include "blocks.h"
#include <iostream>

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
    std::vector<std::vector<int>> calls = {};
    int niters = 0;
    for (auto& vb: vblocks) {
        std::vector<int> call_tmp(vb.children.size(), 0);
        calls.push_back(call_tmp);
        niters += call_tmp.size();
    }

    // Create progressBar to display progress for unpatient asses like mine.
    progressBar pbar_top(niters-1);

    // Check how many times a module children appears in the other rest of
    std::cout << "\n[INFO] - Looking for top modules in module definitions..." << std::endl;
    int i_accum = 0;
    for (int i = 0; i < vblocks.size(); i++) {
        for (int k = 0; k < vblocks[i].children.size(); k++){
            std::string mtmp = string_format("Considering module %s as top-module candidate",vblocks[i].children[k].c_str());
            pbar_top.update(i_accum, mtmp ,"\t");

            // How many times do we find this moddef inside other modules children?
            // (I know it's a bit convoluted, but this is the same as asking if this is a children
            // of any other module)
            for (int j = 0; j < vblocks.size(); j++){
                if (i != j){
                    if (find(vblocks[j].children.begin(), vblocks[j].children.end(), vblocks[i].children[k]) != vblocks[j].children.end()){
                        // We found this module name in the children list of another vblock.
                        // This means this is not a top_module. Add a count to it
                        calls[i][k] += 1;
                        break;
                    }
                }
            }
            i_accum += 1;
        }
    }

    // Print endline
    std::cout << std::endl;

    // Now find the module(s) with the least number of calls
    int min_recurrence = INT_MAX;
    for (auto& c: calls){
        for (auto& cc: c) {
            if (cc < min_recurrence) {
                min_recurrence = cc;
            }
        }
    }

    std::vector<TopModule> top_modules;
    for (int i = 0; i < calls.size(); i++){
        for (int j = 0; j < calls[i].size(); j++) {
            if (calls[i][j] == min_recurrence) {
                // Get module name
                std::string mod_ref_tmp = vblocks[i].children[j];

                // Look for ref in module_definitions
                std::__wrap_iter<VerilogBlock *> index_ref = find_module_def(module_definitions.begin(), module_definitions.end(), mod_ref_tmp);
                if (index_ref != module_definitions.end())
                {
                    std::string mtmp = string_format("\t\tModule %s set as top-module",mod_ref_tmp.c_str());
                    std::cout << mtmp << std::endl;

                    // Create temp top module
                    TopModule tptmp(mod_ref_tmp, &(*index_ref));

                    // Add to structure
                    top_modules.push_back(tptmp);
                }

            }
        }
    }

    // Print endline
    std::cout << std::endl;

    // All top_modules are independent top_modules
    // With this, now we can build our design hierarchy. Let's loop thru top_modules.
    // Our hierarchy in this case will be a json object, which is extremely powerful
    // and useful for us rn
    Json::Value hierarchy;
    for (auto& tpm: top_modules){
        // Get name
        std::string h_name = tpm.name;
        // Create entry for this top_module
        Json::Value tmp = Design::__recursive_seeker__(*(tpm.REF), module_definitions);
        // Append to hierarchy
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