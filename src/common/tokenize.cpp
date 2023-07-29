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


size_t
llcltok::findRight( const std::string &str, size_t left )
{
    int balance = 0;

    for (size_t i=left; i<str.length(); i++)
    {
        if (str[i] == '(')
            balance += 1;
        
        if (str[i] == ')')
        {
            balance -= 1;

            if (balance == 0)
                return i;
        }
    }

    return std::string::npos;
}



// Extract everything from '(' to ')'
std::string
llcltok::extractPar( const std::string &line, size_t left, size_t &right )
{
    right = llcltok::findRight(line, left);

    if (right == std::string::npos)
        return "";

    return line.substr(left+1, left-right-1);
}


// Separate by parentheses, does not look at nested parentheses.
std::vector<std::string>
llcltok::seppar( std::string line )
{
    std::vector<std::string> sep;

    int balance = 0;
    for (size_t i=0; i<line.length(); i++)
    {
        if (line[i] == '(')
        {
            size_t right;
            std::string front = extractPar(line, i, right);
            sep.push_back(front);

            line = line.substr(right+1);
            i = 0;
        }
    }

    return sep;
}


std::vector<std::string>
llcltok::sepws( std::string line )
{
    std::vector<std::string> sep;

    std::stringstream stream(line);
    std::string str;

    while (stream >> str)
        sep.push_back(str);

    return sep;
}


std::string
llcltok::concatTokens( const std::vector<std::string> &tokens, int start, int end, std::string sep )
{
    std::string str = "";

    for (int i=start; i<=end; i++)
    {
        str += tokens[i] + sep;
    }
    str = str.substr(0, str.length()-sep.length());

    return str;
}
