#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stdint.h>


enum class SLTokenType
{
    LITERAL,
    DATATYPE,
    VAR_NAME,

    FN_NAME,
    FN_BEG,
    FN_END,
    RETURN,

    IF_BEGIN,
    IF_END,

    OPERATOR,

    NONE
};


std::ostream &operator << (std::ostream &os, SLTokenType token_type);


SLTokenType SLTokenType_fromstr( std::string token_str );


struct SLToken
{
    SLTokenType     type;
    std::string     value;
};



