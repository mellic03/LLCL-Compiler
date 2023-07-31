#pragma once

#include "../nasmAPI/NASM_Interface.h"
#include "../common/common.h"
#include "function.h"
#include "indentstream.h"

namespace llcl
{
    struct Node;
    class Parser;
};



class llcl::Parser
{
private:
    std::map<std::string, llcl::Function>   m_functions;
    std::string                             m_current_function_name = "NONE";

    std::vector<Symbol>     extract_par(const std::vector<Symbol> &symbols, size_t lpar_pos, size_t &rpar_pos);
    void                    write_parse_tree( ofstream &stream, llcl::Node *node, llcl::Function &function );


    void        parse_function_begin( const llcl::Command &command );
    void        parse_function_end();

    void        write_for_begin( ofstream &stream, Function &function, Command &command );
    void        write_for_end( ofstream &stream, Function &function, Command &command );
    void        write_function( ofstream &stream, std::string name, Function &function );


public:
    llcl::Node *    gen_parse_tree( std::vector<Symbol> expression );

    void        feedcommand( llcl::Command command );


    void        writeNASM( ofstream &stream );
};

