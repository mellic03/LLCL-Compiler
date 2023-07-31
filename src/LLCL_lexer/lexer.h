#pragma once

#include "../common/common.h"

namespace llcl { class Lexer; };


class llcl::Lexer
{
private:
    std::vector<llcl::Command>  m_commands;

    llcl::SymbolClass   deduce_symbol_class( std::string str );
    std::vector<Symbol> tokenize_expression( const std::vector<std::string> &tokens, size_t start = 0 );

    void                tokenize_function_begin( std::vector<std::string> tokens );
    void                tokenize_function_end( std::vector<std::string> tokens );
    void                tokenize_function_return( std::vector<std::string> tokens );

    void                tokenize_for_begin( std::vector<std::string> tokens );
    void                tokenize_for_end( std::vector<std::string> tokens );

    void                tokenize_dec_ass( std::vector<std::string> tokens );
    void                tokenize_ass( std::vector<std::string> tokens );

public:
    void                feedline( std::string line );

    bool                getcommand( llcl::Command &command );
};


