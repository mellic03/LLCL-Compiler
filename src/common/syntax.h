#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stdint.h>



namespace llcl
{
    enum class SymbolClass
    {
        LITERAL, SUBJECT,

        OPERATOR,

        FN_BEG, FN_END,
        WH_BEG, WH_END,
        IF_BEG, IF_END,

        NONE
    };

    struct Symbol
    {
        SymbolClass     m_class;
        std::string     m_value;
    };


    SymbolClass symbolclass_fromstr( std::string token_str );

    bool is_datatype( std::string str );
    bool is_operator( std::string str );
};

std::ostream &operator << (std::ostream &os, llcl::SymbolClass s_class);





