//
// Created by Manuel Blanco Valentin on 1/8/21.
//

#ifndef HLSPY_BLOCKS_H
#define HLSPY_BLOCKS_H

#include <string>
#include <vector>

// Ports structure
struct Ports
{
    std::vector<std::string> input;
    std::vector<std::string> output;
    std::vector<std::string> inout;
};

// Netwire structure (holds regs, wires, ints, etc.)
struct NetWire
{
    std::string name;
    std::string type;
    std::string bitspan;
    std::string arrayspan;
    std::string value;

    // Constructor
    NetWire(std::string Name, std::string Type, std::string BitSpan,
            std::string ArraySpan, std::string Value){
        name = Name;
        type = Type;
        bitspan = BitSpan;
        arrayspan = ArraySpan;
        value = Value;
    }
};

// Parameter structure
struct Parameter
{
    std::string name;
    std::string value;

    // Constructor
    Parameter(std::string Name, std::string Value){
        name = Name;
        value = Value;
    }
};

// VerilogBlock (module, file, whatever)
struct VerilogBlock
{
    std::string ref;
    std::string name;
    std::vector <VerilogBlock> instances;
    std::vector <NetWire> netwires;
    Ports ports;
    std::vector <Parameter> parameters;
    std::vector <std::string> ancestors;
    std::vector <std::string> children;
    std::vector <std::string> inner_moddefs;

    // Initialize (Constructor)
    VerilogBlock(std::string Ref = "block",
                 std::string Name = "",
                 std::vector<VerilogBlock> *Instances = new std::vector<VerilogBlock>(),
                 std::vector<NetWire> *NetWires = new std::vector<NetWire>(),
                 Ports *PORTS = new Ports(),
                 std::vector <Parameter> *Parameters = new std::vector<Parameter>(),
                 std::vector <std::string> Ancestors = {},
                 std::vector <std::string> Children = {},
                 std::vector <std::string> Inner_ModDefs = {}){
        ref = Ref;
        name = Name;
        instances = *Instances;
        netwires = *NetWires;
        ports = *PORTS;
        parameters = *Parameters;
        ancestors = Ancestors;
        children = Children;
        inner_moddefs = Inner_ModDefs;
    }

};

// Top Module
struct TopModule {
    std::string name;
    VerilogBlock* REF;

    // Constructor
    TopModule(std::string Name, VerilogBlock* Ref){
        name = Name;
        REF = Ref;
    }
};

#endif //HLSPY_BLOCKS_H
