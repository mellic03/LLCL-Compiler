#pragma once

#include "../nasmAPI/NASM_Interface.h"
#include "../LLCL_lexer/lexer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdint.h>


enum class CommandType
{
    NONE,
    FUNCTION_DECLARATION
};


class SLParser
{
private:

public:
    std::vector<std::vector<SLToken>> m_tokens;
    void loadTokens(std::string filepath);

};


