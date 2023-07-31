#include "parser.h"

const std::string LLCL_FN_PREFIX = "LLCL_FN_";


std::vector<llcl::Symbol>
llcl::Parser::extract_par(const std::vector<Symbol> &symbols, size_t lpar_pos, size_t &rpar_pos)
{
    std::vector<Symbol> symbols_out;

    int balance = 0;
    for (size_t i = lpar_pos; i < symbols.size(); i++)
    {
        const Symbol &symbol = symbols[i];

        if (symbol.m_class == SymbolClass::LPAR)
            balance += 1;

        else if (symbol.m_class == SymbolClass::RPAR)
        {
            balance -= 1;
            rpar_pos = i;
            if (balance == 0)
                return symbols_out;
        }

        else
        {
            symbols_out.push_back(symbol);
        }
    }

    std::cout << "llcl::Parser::extract_par():    Error matching parentheses\n";
    exit(1);

    return symbols_out;
}



llcl::Node *
llcl::Parser::gen_parse_tree( std::vector<llcl::Symbol> expr )
{
    // Firstly, if entire expression is in parentheses, remove them --------------
    bool left_is_par  = expr.front().m_class == SymbolClass::LPAR;
    bool right_is_par = expr.back().m_class  == SymbolClass::RPAR;

    while (left_is_par && right_is_par)
    {
        expr.pop_back();
        expr.erase(expr.begin());

        left_is_par  = expr.front().m_class == SymbolClass::LPAR;
        right_is_par = expr.back().m_class  == SymbolClass::RPAR;
    }
    // ---------------------------------------------------------------------------

    llcl::Node *node = new llcl::Node();

    // Base case, expression is a single subject or literal
    if (expr.size() == 1)
    {
        node->m_symbol = expr[0];
        return node;
    }

    int balance = 0;

    for (size_t i=0; i<expr.size(); i++)
    {
        const llcl::Symbol &symbol = expr[i];

        if (symbol.m_class == SymbolClass::LPAR)
            balance += 1;

        if (symbol.m_class == SymbolClass::RPAR)
            balance -= 1;

        if (symbol.m_class == SymbolClass::OPERATOR && balance == 0)
        {
            node->m_symbol = symbol;

            auto left  = std::vector<Symbol>(expr.begin(), expr.begin()+i);
            auto right = std::vector<Symbol>(expr.begin()+i+1, expr.end());

            node->left  = gen_parse_tree(left);
            node->right = gen_parse_tree(right);

            return node;
        }
    }

    delete node;
    std::cout << "llcl::Parser::gen_parse_tree():  This should not execute!!!\n";
    return nullptr;
}



void
llcl::Parser::parse_function_begin( const llcl::Command &command )
{
    const auto &symbols = command.m_symbols;

    std::string function_name = LLCL_FN_PREFIX + symbols[1].m_value;
    m_functions[function_name] = llcl::Function();
    m_current_function_name = function_name;

    size_t rpar_pos;
    std::vector<Symbol> params = extract_par(symbols, 2, rpar_pos);
    for (int i=0; i<params.size(); i+=2)
    {
        Type datatype = llcl::type_fromstr(params[i].m_value);
        std::string param_name = params[i+1].m_value;
        m_functions[function_name].add_param(datatype, param_name);
    }
}


void
llcl::Parser::parse_function_end()
{
    llcl::Function &function = m_functions[m_current_function_name];

    // Now that we have the entire function body,
    // we can run through and determine how much
    // space is needed for local variables

    size_t byte_offset = 8;

    for (auto &command: function.m_commands)
    {
        bool c1 = command.m_class == CommandClass::DEC;
        bool c2 = command.m_class == CommandClass::DEC_ASS;

        if (c1 || c2)
        {
            std::string var_name = command.m_symbols[1].m_value;
            std::string type_str = command.m_symbols[0].m_value;
            Type dtype = llcl::type_fromstr(type_str);

            function.add_param(dtype, var_name);
            byte_offset += llcl::typesize(dtype);
            function.m_variables[var_name].m_byte_offset = byte_offset;
        }
    }

    for (auto &[name, var]: function.m_variables)
    {
        function.bytes_required  += llcl::typesize(var.m_type);
    }

    m_current_function_name = "NONE";
}



void
llcl::Parser::feedcommand( llcl::Command command )
{

    switch (command.m_class)
    {
        // case CommandClass::DEC:         break;
        // case CommandClass::ASS:         break;
        // case CommandClass::DEC_ASS:     break;

        case CommandClass::FN_BEG:      parse_function_begin(command);  break;
        case CommandClass::FN_END:      parse_function_end();           break;

        // case CommandClass::WH_BEG:      break;
        // case CommandClass::WH_END:      break;


        // case CommandClass::FR_BEG:      parse_for_begin(command);       break;
        // case CommandClass::FR_END:      parse_for_end();                break;

        // case CommandClass::IF_BEG:      break;
        // case CommandClass::IF_END:      break;

        // case CommandClass::RETURN:      break;

        default:
            if (m_current_function_name != "NONE")
                m_functions[m_current_function_name].push_command(command);
            break;
    }


}
