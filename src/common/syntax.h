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
        FR_BEG, FR_END,
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

        DEC_ASS_FN_CALL,    // i64 p_vec3 = alloc(3)
        ASS_FN_CALL,        // p_vec3 = alloc(3)
        FN_CALL,            // foo(x, y, z)
    
        FN_BEG, FN_END, BUILTIN_CALL,
        WH_BEG, WH_END,
        FR_BEG, FR_END,
        IF_BEG, IF_END,

        RETURN,

        NONE
    };

    struct Command
    {
       CommandClass             m_class;
       std::vector<Symbol>      m_symbols; 
    };

    enum class Type
    {
        u00,
        u08, u16, u32, u64,
        i08, i16, i32, i64,
        f32, f64
    };

    SymbolClass     symbolclass_fromstr( std::string str );
    CommandClass    commandclass_fromstr( std::string str );
    Type            type_fromstr( std::string str );

    bool is_datatype( std::string str );
    bool is_operator( std::string str );
    bool is_literal( std::string str );

    size_t typesize( Type dtype );

    struct Node
    {
        llcl::Symbol m_symbol;
        Node *left  = nullptr;
        Node *right = nullptr;
    };

    void print_tree( std::string prefix, Node *node, bool is_left );

};

std::ostream &operator << (std::ostream &os, llcl::SymbolClass s_class);
std::ostream &operator << (std::ostream &os, llcl::CommandClass c_class);





