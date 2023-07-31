#include "lexer.h"


llcl::SymbolClass
llcl::Lexer::deduce_symbol_class( std::string str )
{
    if (str == "fn")        return SymbolClass::FN_BEG;
    if (str == "endfn")     return SymbolClass::FN_END;
    if (str == "wh")        return SymbolClass::WH_BEG;
    if (str == "endwh")     return SymbolClass::WH_END;
    if (str == "if")        return SymbolClass::IF_BEG;
    if (str == "endif")     return SymbolClass::IF_END;
    if (str == "return")    return SymbolClass::RETURN;
    if (str == "for")       return SymbolClass::FR_BEG;
    if (str == "endfor")    return SymbolClass::FR_END;
    if (is_datatype(str))   return SymbolClass::DTYPE;

    return SymbolClass::SUBJECT;
}


std::vector<llcl::Symbol>
llcl::Lexer::tokenize_expression( const std::vector<std::string> &tokens, size_t start )
{
    // a + (b - c) * (15 - d)
    std::vector<llcl::Symbol> symbols;

    for (int i=start; i<tokens.size(); i++)
    {
        SymbolClass s_class = SymbolClass::NONE;

        if (tokens[i] == "(")
            s_class = SymbolClass::LPAR;

        else if (tokens[i] == ")")
            s_class = SymbolClass::RPAR;

        else if (llcl::is_operator(tokens[i]))
            s_class = SymbolClass::OPERATOR;

        else if (llcl::is_datatype(tokens[i]))
            s_class = SymbolClass::DTYPE;
        
        else if (llcl::is_literal(tokens[i]))
            s_class = SymbolClass::LITERAL;

        else
            s_class = SymbolClass::SUBJECT;

        symbols.push_back({s_class, tokens[i]});
    }

    return symbols;
}



void
llcl::Lexer::tokenize_function_begin( std::vector<std::string> tokens )
{
    llcl::Command command;
    command.m_class = CommandClass::FN_BEG;

    command.m_symbols.push_back({llcl::SymbolClass::FN_BEG, "fn"});
    command.m_symbols.push_back({llcl::SymbolClass::SUBJECT, tokens[1]});

    for (int i=2; i<tokens.size(); i++)
    {
        if (tokens[i] == "(")
        {
            command.m_symbols.push_back({SymbolClass::LPAR, tokens[i]});
        }

        else if (tokens[i] == ")")
        {
            command.m_symbols.push_back({SymbolClass::RPAR, tokens[i]});
        }

        else if (llcl::is_datatype(tokens[i]))
        {
            command.m_symbols.push_back({SymbolClass::DTYPE, tokens[i]});
        }

        else
        {
            command.m_symbols.push_back({SymbolClass::SUBJECT, tokens[i]});
        }
    }

    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_function_end( std::vector<std::string> tokens )
{
    llcl::Command command;
    command.m_class = CommandClass::FN_END;
    command.m_symbols.push_back({SymbolClass::FN_END, "endfn"});
    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_function_return( std::vector<std::string> tokens )
{
    llcl::Command command;
    command.m_class = CommandClass::RETURN;

    // Remove first element "return",
    // leaving only the expression to be returned.
    command.m_symbols = tokenize_expression(tokens, 1);
    command.m_symbols.insert(
        command.m_symbols.begin(),
        { SymbolClass::RETURN, "return" }
    );

    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_for_begin( std::vector<std::string> tokens )
{
    llcl::Command command;
    command.m_class = CommandClass::FR_BEG;

    command.m_symbols.push_back({SymbolClass::FR_BEG, tokens[0]});
    command.m_symbols.push_back({SymbolClass::LITERAL, tokens[1]});
    command.m_symbols.push_back({SymbolClass::LITERAL, tokens[3]});

    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_for_end( std::vector<std::string> tokens )
{
    llcl::Command command;
    command.m_class = CommandClass::FR_END;
    command.m_symbols.push_back({SymbolClass::FR_END, tokens[0]});
    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_dec_ass( std::vector<std::string> tokens )
{
    llcl::Command command;
    if (tokens.size() == 2)
        command.m_class = CommandClass::DEC;
    else
        command.m_class = CommandClass::DEC_ASS;
    command.m_symbols = tokenize_expression(tokens);

    m_commands.push_back(command);
}


void
llcl::Lexer::tokenize_ass( std::vector<std::string> tokens )
{
    // data = a + b + c + ...
    // left = right

    llcl::Command command;
    command.m_class = CommandClass::ASS;
    command.m_symbols = tokenize_expression(tokens, 2);

    command.m_symbols.insert(
        command.m_symbols.begin(),
        {SymbolClass::OPERATOR, tokens[1]}
    );

    command.m_symbols.insert(
        command.m_symbols.begin(),
        {SymbolClass::SUBJECT, tokens[0]}
    );

    m_commands.push_back(command);
}


void
llcl::Lexer::feedline( std::string line )
{
    auto tokens = llcltok::sepws(line);
    if (tokens.size() == 0)
        return;


    llcl::SymbolClass s_class = deduce_symbol_class(tokens[0]);

    switch (s_class)
    {
        case SymbolClass::FN_BEG:   tokenize_function_begin(tokens);    break;
        case SymbolClass::FN_END:   tokenize_function_end(tokens);      break;

        case SymbolClass::WH_BEG:                                       break;
        case SymbolClass::WH_END:                                       break;

        case SymbolClass::FR_BEG:   tokenize_for_begin(tokens);         break;
        case SymbolClass::FR_END:   tokenize_for_end(tokens);           break;

        case SymbolClass::IF_BEG:                                       break;
        case SymbolClass::IF_END:                                       break;

        case SymbolClass::RETURN:   tokenize_function_return(tokens);   break;

        case SymbolClass::DTYPE:    tokenize_dec_ass(tokens);           break;
        case SymbolClass::SUBJECT:  tokenize_ass(tokens);               break;

        default:    break;
    }

}



bool
llcl::Lexer::getcommand( llcl::Command &command )
{
    static int idx = 0;

    if (idx == m_commands.size())
        return false;

    command = m_commands[idx];
    idx += 1;

    return true;
}
