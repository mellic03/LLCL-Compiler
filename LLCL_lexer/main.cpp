
#include "lexer.h"

std::set<std::string> SL_types {
    "u00",
    "u08", "u16", "u32", "u64",
    "i08", "i16", "i32", "i64"
};


std::set<std::string> SL_operators {
    "=",
    "+", "-", "/", "*",
    "==", "<", ">"
};


std::vector<SLToken> tokenize_function( std::string line )
{
    std::vector<SLToken> tokens;

    for (char &c: line)
        if (c == '(' || c == ')')
            c = ' ';
    
    std::stringstream stream(line);
    std::vector<std::string> sep;
    std::string str;

    while (stream >> str)
        sep.push_back(str);

    // count number of DATATYPE ocurrences, fn has ocurrences-1 parameters
    int num_params = 0;
    for (auto &sss: sep)
        if (SL_types.find(sss) != SL_types.end())
            num_params += 1;
    num_params -= 1;

    // "fn" + function name
    tokens.push_back( {SLTokenType::FN_BEG, sep[0]} );
    tokens.push_back( {SLTokenType::FN_NAME, sep[1]} );

    // function parameters
    for (int i=0; i<num_params; i+=1)
    {
        tokens.push_back( {SLTokenType::DATATYPE, sep[2+2*i+0]} );
        tokens.push_back( {SLTokenType::VAR_NAME, sep[2+2*i+1]} );
    }

    // return type
    tokens.push_back( {SLTokenType::DATATYPE, sep.back()} );

    return tokens;
}


bool is_number( std::string str )
{
    for (const char c: str)
        if (std::isdigit(c) == false)
            return false;
    return true;
}


bool is_pure_alpha( std::string str )
{
    for (const char c: str)
        if (std::isalpha(c) == false)
            return false;
    return true;
}


bool is_operator( std::string str )
{
    return SL_operators.find(str) != SL_operators.end();
}


std::vector<SLToken> tokenize_generic( std::vector<std::string> ws_sep )
{
    std::vector<SLToken> tokens;
    for (int i=0; i<ws_sep.size(); i++)
    {
        SLTokenType token_type = SLTokenType::NONE;

        if (is_number(ws_sep[i]))
            token_type = SLTokenType::LITERAL;

        else if (is_pure_alpha(ws_sep[i]))
            token_type = SLTokenType::VAR_NAME;

        else if (is_operator(ws_sep[i]))
            token_type = SLTokenType::OPERATOR;

        if (token_type != SLTokenType::NONE)
            tokens.push_back( {token_type, ws_sep[i]} );
    }
    return tokens;
}


std::vector<SLToken> tokenize_dec_ass( std::vector<std::string> ws_sep )
{
    std::vector<SLToken> tokens;

    tokens.push_back( {SLTokenType::DATATYPE, ws_sep[0]} ); // datatype
    tokens.push_back( {SLTokenType::VAR_NAME, ws_sep[1]} ); // name
    tokens.push_back( {SLTokenType::OPERATOR, ws_sep[2]} ); // assignment operator


    std::vector<SLToken> moretokens = tokenize_generic(
        std::vector<std::string>(ws_sep.begin()+3, ws_sep.end())
    );

    std::copy(
        moretokens.begin(), moretokens.end(),
        std::back_inserter(tokens)
    );

    return tokens;
}


std::vector<SLToken> tokenize_beg_if( std::vector<std::string> ws_sep )
{
    std::vector<SLToken> tokens;

    tokens.push_back( {SLTokenType::IF_BEGIN, "if"} );

    std::vector<SLToken> moretokens = tokenize_generic(
        std::vector<std::string>(ws_sep.begin()+1, ws_sep.end())
    );

    std::copy(
        moretokens.begin(), moretokens.end(),
        std::back_inserter(tokens)
    );

    return tokens;
}


std::vector<SLToken> tokenize_return( std::vector<std::string> ws_sep )
{
    std::vector<SLToken> tokens;

    tokens.push_back( {SLTokenType::RETURN, "return"} );

    std::vector<SLToken> moretokens = tokenize_generic(
        std::vector<std::string>(ws_sep.begin()+1, ws_sep.end())
    );

    std::copy(
        moretokens.begin(), moretokens.end(),
        std::back_inserter(tokens)
    );

    return tokens;
}


std::vector<SLToken> tokenize( std::string line )
{
    std::vector<SLToken> tokens;

    std::stringstream stream(line);
    std::vector<std::string> sep;
    std::string str;

    while (stream >> str)
        sep.push_back(str);

    if (sep.size() == 0)
        return tokens;

    SLTokenType last_type = SLTokenType::NONE;

    // no need to check each token in
    // a generic way if line starts with "fn"
    if (sep[0] == "fn")
    {
        return tokenize_function(line);
    }

    if (sep[0] == "endfn")
    {
        return { {SLTokenType::FN_END, "endfn"} };
    }

    if (sep[0] == "if")
    {
        return tokenize_beg_if(sep);
    }

    if (sep[0] == "endif")
    {
        return { {SLTokenType::IF_END, "endif"} };
    }

    if (sep[0] == "return")
    {
        return tokenize_return(sep);
    }

    // If first token is datatype, line is either DEC or DEC + ASS
    if (SL_types.find(sep[0]) != SL_types.end())
    {
        // If only two tokens, then line must be declaration only
        if (sep.size() == 2)
        {
            return {
                {SLTokenType::DATATYPE, sep[0]},
                {SLTokenType::VAR_NAME, sep[1]}
            };
        }

        return tokenize_dec_ass(sep);
    }

    return tokens;
}


int main(int argc, char **argv)
{
    std::ifstream instream(argv[1]);
    std::ofstream outstream(argv[2]);
    std::string line;

    while (std::getline(instream, line))
    {
        std::vector<SLToken> tokens = tokenize(line);

        if (tokens.size() == 0)
            continue;

        for (SLToken token: tokens)
            outstream << "<" << token.type << ", " << token.value << "> ";
        outstream << std::endl;
    }

    return 0;
}

