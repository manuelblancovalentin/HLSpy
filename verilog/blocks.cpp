//
// Created by Manuel Blanco Valentin on 1/12/21.
//


#include "blocks.h"
#include "design.h"

void VerilogBlock::__build_subhierarchy__(std::string TAB = ""){

    // Get all fields
    subhierarchy = "";
    //std::vector<Parameter>* tmp_params = &(this->parameters);
    //Ports* tmp_ports = &(this->ports);
    //std::vector<NetWire>* tmp_netwires = &(this->netwires);
    //std::vector<VerilogBlock>* tmp_instances = &(this->instances);

    bool print_params = !parameters.empty();
    bool print_ports = !ports.input.empty() || !ports.output.empty() || !ports.inout.empty();
    bool print_netwires = !netwires.empty();
    bool print_instances = !instances.empty();

    // Place reference
    subhierarchy += string_format("%s\"ref\" : \"%s\",\n", TAB.c_str(), name.c_str());
    //tmp["ref"] = ref;

    // Parse parameters
    if (print_params){
        // Open parameters field
        subhierarchy += string_format("%s\"parameters\" : \n%s{\n", TAB.c_str(), TAB.c_str());
        int pmax = parameters.size();
        for (int p = 0; p < pmax; p++){
            subhierarchy += string_format("%s\"%s\" : \"%s\"", TAB.c_str() + '\t',
                                       parameters[p].name.c_str(),
                                       parameters[p].value.c_str());
            if (p != (pmax-1)){
                subhierarchy += ",";
            }
            subhierarchy += "\n";
        }
        // Close parameters field
        subhierarchy += string_format("%s}", TAB.c_str());

        if (print_ports|print_netwires|print_instances){
            subhierarchy += ",";
        }
        subhierarchy += "\n";
    }

    // Ports
    // Open field
    if (print_ports){
        subhierarchy += string_format("%s\"ports\" : \n%s{\n", TAB.c_str(), TAB.c_str());
        // Inputs
        //Json::Value tmp_iports(Json::arrayValue);
        // Open inputs
        subhierarchy += string_format("%s\"input\" : \n%s[\n", (TAB + "\t").c_str() , (TAB + "\t").c_str());
        int pmax = ports.input.size();
        for (int p = 0; p < pmax; p++){
            subhierarchy += string_format("%s\"%s\"", (TAB + "\t\t").c_str(), ports.input[p].c_str());
            if (p != (pmax-1)){
                subhierarchy += ",";
            }
            subhierarchy += "\n";
        }

        // Close input field
        subhierarchy += string_format("%s],\n", (TAB + "\t").c_str());

        // Open outputs
        subhierarchy += string_format("%s\"output\" : \n%s[\n", (TAB + "\t").c_str(), (TAB + "\t").c_str());
        pmax = ports.output.size();
        for (int p = 0; p < pmax; p++){
            subhierarchy += string_format("%s\"%s\"", (TAB + "\t\t").c_str(), ports.output[p].c_str());
            if (p != (pmax-1)){
                subhierarchy += ",";
            }
            subhierarchy += "\n";
        }
        // Close output field
        subhierarchy += string_format("%s],\n", (TAB + "\t").c_str());

        // Open inout
        subhierarchy += string_format("%s\"inout\" : \n%s[\n", (TAB + "\t").c_str(), (TAB + "\t").c_str());
        pmax = ports.inout.size();
        for (int p = 0; p < pmax; p++){
            subhierarchy += string_format("%s\"%s\"", (TAB + "\t\t").c_str(), ports.inout[p].c_str());
            if (p != (pmax-1)){
                subhierarchy += ",";
            }
            subhierarchy += "\n";
        }
        // Close inout field
        subhierarchy += string_format("%s]\n", (TAB + "\t").c_str());

        // Close ports field
        subhierarchy += string_format("%s}", TAB.c_str());

        if (print_netwires|print_instances){
            subhierarchy += ",";
        }
        subhierarchy += "\n";
    }


    // Netwires
    if (print_netwires) {
        int nwmax = netwires.size();
        for (int nw = 0; nw < nwmax; nw++) {
            // Open netwire field
            subhierarchy += string_format("%s\"%s\" : \n%s{\n", TAB.c_str(), netwires[nw].name.c_str(),
                                       TAB.c_str());

            // Construct values
            //tmp[nw.name]["type"] = nw.type;
            subhierarchy += string_format("%s\"type\" : \"%s\",\n", (TAB + "\t").c_str(),
                                       netwires[nw].type.c_str());

            //tmp[nw.name]["bitspan"] = nw.bitspan;
            subhierarchy += string_format("%s\"bitspan\" : \"%s\",\n", (TAB + "\t").c_str(),
                                       netwires[nw].bitspan.c_str());

            //tmp[nw.name]["arrayspan"] = nw.arrayspan;
            subhierarchy += string_format("%s\"arrayspan\" : \"%s\",\n", (TAB + "\t").c_str(),
                                       netwires[nw].arrayspan.c_str());

            //tmp[nw.name]["value"] = nw.value;
            subhierarchy += string_format("%s\"value\" : \"%s\"\n", (TAB + "\t").c_str(),
                                       netwires[nw].value.c_str());

            // Close netwire field
            subhierarchy += string_format("%s}", TAB.c_str());

            if ((nw != (nwmax - 1)) | print_instances) {
                subhierarchy += ",";
            }
            subhierarchy += "\n";
        }
    }

};




// Instance push
bool VerilogBlock::push(std::string ref, VerilogBlock value){

    // Check if string is present
    if (instances.contains(ref)){
        // Get array from structure
        instances.at(ref).push_back(value);
        return true;
    } else {
        // Create vector
        instances.insert(std::pair<std::string, std::vector<VerilogBlock>>(ref, std::vector({value})));
        return false;
    }

}