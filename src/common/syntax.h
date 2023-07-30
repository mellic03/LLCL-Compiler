#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stdint.h>


/*
    ----------------------- Backus-Naur Form (BNF) -----------------------
    ----------------------------------------------------------------------

    <expression>    ::=     <subject>  |  <literal>
    <expression>    ::=     <expression>  <operator>  <expression>

    <command>       ::=     <type>  <subject>
    <command>       ::=     <type>  <subject>  =  <expression>
    <command>       ::=     <subject>  =  <expression>
    <command>       ::=     return  <expression>
    ----------------------------------------------------------------------
*/


namespace llcl
{
    enum class SymbolClass
    {
        LITERAL, SUBJECT,
        
        LPAR, RPAR, // '(', ')'

        DTYPE,      // datatype, i64, f32 etc
        
        OPERATOR,

        FN_BEG, FN_END,
        WH_BEG, WH_END,
        IF_BEG, IF_END,

        RETURN,

        NONE
    };

    struct Symbol
    {
        SymbolClass     m_class;
        std::string     m_value;
    };

    enum class CommandClass
    {
        DEC, ASS, DEC_ASS,

        FN_BEG, FN_END,
        WH_BEG, WH_END,
        IF_BEG, IF_END,

        RETURN,

        NONE
    };

    struct Command
    {
       CommandClass             m_class;
       std::vector<Symbol>      m_symbols; 
    };

    SymbolClass     symbolclass_fromstr( std::string str );
    CommandClass    commandclass_fromstr( std::string str );

    bool is_datatype( std::string str );
    bool is_operator( std::string str );
    bool is_literal( std::string str );
};

std::ostream &operator << (std::ostream &os, llcl::SymbolClass s_class);
std::ostream &operator << (std::ostream &os, llcl::CommandClass c_class);





