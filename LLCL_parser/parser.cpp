#include "parser.h"


std::vector<SLToken> loadTokenLine( std::string line )
{
    std::vector<std::string> delimited;

    size_t rbracket_pos = line.find(">");

    while (rbracket_pos != std::string::npos)
    {
        std::string head = line.substr(1, rbracket_pos-1);

        delimited.push_back(head);
        line = line.substr(rbracket_pos+2);

        rbracket_pos = line.find(">");
    }

    std::vector<SLToken> tokens;
    for (const auto &str: delimited)
    {
        std::string token_str   = str.substr(0, str.find(","));
        std::string token_value = str.substr(str.find(",")+2);

        tokens.push_back(
            {
                SLTokenType_fromstr(token_str),
                token_value
            }
        );
    }

    return tokens;
}


void
SLParser::loadTokens( std::string filepath )
{
    std::ifstream stream(filepath);
    std::string line;

    while (std::getline(stream, line))
    {
        m_tokens.push_back(loadTokenLine(line));
    }
}