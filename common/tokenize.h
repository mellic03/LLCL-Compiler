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


    // Extract everything from '(' to ')'
    std::string                 extractPar( const std::string &line, size_t left );

    // Separate by parentheses
    std::vector<std::string>    seppar( std::string line );
};

