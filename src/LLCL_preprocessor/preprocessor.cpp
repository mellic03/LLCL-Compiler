#include "preprocessor.h"



std::string &
LLCLPP::remove_comment( std::string &line )
{
    // Remove comments
    size_t comment_pos = line.find("//");
    if (comment_pos != std::string::npos)
    {
        line = line.substr(0, comment_pos);
    }

    return line;
}


std::string &
LLCLPP::pad_whitespace( std::string &line )
{
    bool in_string = false;

    for (size_t i=0; i<line.length(); i++)
    {
        if (line[i] == '\"')
            in_string = !in_string;

        if (in_string)
            continue;

        else if (line[i] == '(' || line[i] == ')')
        {
            line.insert(i+1, " ");
            line.insert(i,   " ");
            i += 2;
        }
    }

    return line;
}


std::string &
LLCLPP::replace_semicolons( std::string &line )
{
    bool in_string = false;

    for (size_t i=0; i<line.length(); i++)
    {
        if (line[i] == '\"')
            in_string = !in_string;

        if (in_string)
            continue;

        else if (line[i] == ';')
        {
            line[i] = '\n';
        }
    }

    return line;
}



bool
LLCLPP::is_basic_macro_declaration( std::string line )
{
    return line.find("#define") != std::string::npos;
}


void
LLCLPP::load_basic_macro_declaration( std::string &line )
{
    auto tokens = llcltok::sepws(line);

    //  0        1     2    3..       ..n
    //  #define  name  (    value       )
    //  #define  MIDW  (    50 * 1000   )

    std::string name = tokens[1];
    std::string str = llcltok::concatTokens(tokens, 2, tokens.size()-1, "");

    m_basic_macros[name] = str;

    line = "";
}




bool
LLCLPP::is_param_macro_declaration( std::string line )
{
    auto tokens = llcltok::seppar(line);

    bool cond1 = line.find("#define") == 0;
    bool cond2 = tokens.size() >= 2;

    return cond1 && cond2;
}


void
LLCLPP::load_param_macro_declaration( std::string &line )
{
    auto tokens = llcltok::sepws(line);


    //  0        1     2    3..       ..n
    //  #define  name  (    value       )
    //  #define  MIDW  (    50 * 1000   )

    std::string name = tokens[1];
    std::string parameter = tokens[2].substr(1, tokens[2].length()-2);
    std::string value = llcltok::concatTokens(tokens, 3, tokens.size()-1, "");
    value = value.substr(1, value.length()-2);

    std::cout << "MACRO STR: " << value << " \n";
    std::cout << "param: " << parameter << "\n";

    LLCL_param_macro macro = {
        value,
        parameter,
        value.find(parameter)
    };

    m_param_macros[name] = macro;

    line = "";
}



bool
LLCLPP::find_basic_macro_reference( std::string line, std::string &macro_name, size_t &pos )
{
    for (auto &[name, value]: m_basic_macros)
    {
        pos = line.find(name);
        if (pos != std::string::npos)
        {
            // ensure macro name isn't actually part of another token
            if (pos != 0 && line[pos-1] != ' ')
                return false;

            macro_name = name;
            return true;
        }
    }

    return false;
}


void
LLCLPP::replace_basic_macro_reference( std::string &line, std::string macro_name, size_t pos )
{
    line.erase(pos, macro_name.length());
    line.insert(pos, m_basic_macros[macro_name]);
}




bool
LLCLPP::find_param_macro_reference( std::string line, std::string &macro_name, size_t &pos )
{
    for (auto &[name, value]: m_param_macros)
    {
        pos = line.find(name);
        if (pos != std::string::npos)
        {
            if (pos != 0 && line[pos-1] != ' ')
                return false;

            macro_name = name;
            return true;
        }
    }

    return false;
}


void
LLCLPP::replace_param_macro_reference( std::string &line, std::string macro_name, size_t pos )
{
    LLCL_param_macro macro = m_param_macros[macro_name];

    size_t left = pos + macro_name.length();
    size_t right = llcltok::findRight(line, left);
    std::string user_param = line.substr(left+1, right-left-1);


    line.erase(pos, right-pos+1);
    line.insert(pos, macro.genStr(user_param));
    // size_t right = llcltok::findRight(line, pos);
    // line.erase(pos, right-pos+1);

    // line.insert(pos, macro.genStr("addr"));

}


