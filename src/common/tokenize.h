#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdint.h>


namespace llcltok
{
    std::string &               ltrim( std::string &str );
    std::string &               rtrim( std::string &str );
    std::string &               stripws( std::string &str );


    /** Given the index of a '(', return the index of it's matching ')'
    */
    size_t                      findRight( const std::string &str, size_t left );


    /** Extract everything from '(' to ')'
     * @param left  starting '('
     * @param right returns ending ')'
    */
    std::string                 extractPar( const std::string &line, size_t left, size_t &right );

    // Separate by parentheses
    std::vector<std::string>    seppar( std::string line );

    // Separate by whitespace
    std::vector<std::string>    sepws( std::string line );


    std::string concatTokens( const std::vector<std::string> &tokens, int start, int end, std::string sep = "" );
};

