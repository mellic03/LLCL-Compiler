#include "tokenize.h"

std::string &
llcltok::ltrim( std::string &str )
{
    size_t pos = 0;
    while (str[pos] == ' ' && pos < str.length())
    {
        pos += 1;
    }
    str = str.substr(pos);
    return str;
}


std::string &
llcltok::rtrim( std::string &str )
{
    size_t pos = str.length() - 1;
    while (str[pos] == ' ' && pos > 0)
    {
        pos -= 1;
    }
    str = str.substr(0, pos+1);
    return str;
}


std::string &
llcltok::stripws( std::string &str )
{
    return ltrim(rtrim(str));
}


// Extract everything from '(' to ')'
std::string
llcltok::extractPar( const std::string &line, size_t left )
{
    int balance = 0;

    for (size_t i=left; i<line.length(); i++)
    {
        if (line[i] == '(')
            balance += 1;
        
        if (line[i] == ')')
        {
            balance -= 1;

            if (balance == 0)
            {
                return line.substr(left+1, i-left-1);
            }
        }
    }

    return "";
}


// Separate by parentheses
std::vector<std::string>
llcltok::seppar( std::string line )
{
    std::vector<std::string> sep;

    int balance = 0;
    for (size_t i=0; i<line.length(); i++)
    {
        if (line[i] == '(')
        {
            sep.push_back(extractPar(line, i));
        }
    }

    return sep;
}
