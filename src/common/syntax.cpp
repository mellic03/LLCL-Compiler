#include "syntax.h"

const std::set<std::string> llcl_datatypes {
    "u00",
    "u08", "u16", "u32", "u64",
    "i08", "i16", "i32", "i64"
};

const std::set<std::string> llcl_operators {
    "=",
    "+", "-", "/", "*",
    "==", "<", ">"
};


bool
llcl::is_datatype( std::string str )
{
    return llcl_datatypes.find(str) != llcl_datatypes.end();
};


bool
llcl::is_operator( std::string str )
{
    return llcl_operators.find(str) != llcl_operators.end();
};


bool
llcl::is_literal( std::string str )
{
    if (str.front() == '\"' && str.back() == '\"')
        return true;

    for (char c: str)
        if (c < '0' || c > '9')
            return false;

    return true;
};


size_t
llcl::typesize( Type dtype )
{
    switch (dtype)
    {
        case Type::u08:
        case Type::i08:   return 1;

        case Type::u16:   return 2;
        case Type::i16:   return 2;

        case Type::u32:   return 4;
        case Type::i32:   return 4;
        case Type::f32:   return 4;

        case Type::u64:   return 8;
        case Type::i64:   return 8;
        case Type::f64:   return 8;
    }

    return 0;
}



std::ostream &operator << (std::ostream &os, llcl::SymbolClass s_class)
{
    switch (s_class)
    {
        case llcl::SymbolClass::LITERAL:    os << "LITERAL";    break;
        case llcl::SymbolClass::SUBJECT:    os << "SUBJECT";    break;
        case llcl::SymbolClass::OPERATOR:   os << "OPERATOR";   break;
        case llcl::SymbolClass::LPAR:       os << "LPAR";       break;
        case llcl::SymbolClass::RPAR:       os << "RPAR";       break;
        case llcl::SymbolClass::DTYPE:      os << "DTYPE";      break;
        case llcl::SymbolClass::FN_BEG:     os << "FN_BEG";     break;
        case llcl::SymbolClass::FN_END:     os << "FN_END";     break;
        case llcl::SymbolClass::WH_BEG:     os << "WH_BEG";     break;
        case llcl::SymbolClass::WH_END:     os << "WH_END";     break;
        case llcl::SymbolClass::FR_BEG:     os << "FR_BEG";     break;
        case llcl::SymbolClass::FR_END:     os << "FR_END";     break;
        case llcl::SymbolClass::IF_BEG:     os << "IF_BEG";     break;
        case llcl::SymbolClass::IF_END:     os << "IF_END";     break;
        case llcl::SymbolClass::RETURN:     os << "RETURN";     break;
        case llcl::SymbolClass::NONE:       os << "NONE";       break;
    }

    return os;
}


std::ostream &operator << (std::ostream &os, llcl::CommandClass c_class)
{
    switch (c_class)
    {
        case llcl::CommandClass::DEC:       os << "DEC";        break;
        case llcl::CommandClass::ASS:       os << "ASS";        break;
        case llcl::CommandClass::DEC_ASS:   os << "DEC_ASS";    break;
        case llcl::CommandClass::FN_BEG:    os << "FN_BEG";     break;
        case llcl::CommandClass::FN_END:    os << "FN_END";     break;
        case llcl::CommandClass::WH_BEG:    os << "WH_BEG";     break;
        case llcl::CommandClass::WH_END:    os << "WH_END";     break;
        case llcl::CommandClass::FR_BEG:    os << "FR_BEG";     break;
        case llcl::CommandClass::FR_END:    os << "FR_END";     break;
        case llcl::CommandClass::IF_BEG:    os << "IF_BEG";     break;
        case llcl::CommandClass::IF_END:    os << "IF_END";     break;
        case llcl::CommandClass::RETURN:    os << "RETURN";     break;
    }

    return os;
}


llcl::SymbolClass llcl::symbolclass_fromstr( std::string str )
{
    if (str == "LITERAL")       return SymbolClass::LITERAL;
    if (str == "SUBJECT")       return SymbolClass::SUBJECT;
    if (str == "OPERATOR")      return SymbolClass::OPERATOR;
    if (str == "LPAR")          return SymbolClass::LPAR;
    if (str == "RPAR")          return SymbolClass::RPAR;
    if (str == "FN_BEG")        return SymbolClass::FN_BEG;
    if (str == "FN_END")        return SymbolClass::FN_END;
    if (str == "WH_BEG")        return SymbolClass::WH_BEG;
    if (str == "WH_END")        return SymbolClass::WH_END;
    if (str == "FR_BEG")        return SymbolClass::FR_BEG;
    if (str == "FR_END")        return SymbolClass::FR_END;
    if (str == "IF_BEG")        return SymbolClass::IF_BEG;
    if (str == "IF_END")        return SymbolClass::IF_END;
    if (str == "IF_END")        return SymbolClass::IF_END;

    return SymbolClass::NONE;
}


llcl::CommandClass llcl::commandclass_fromstr( std::string str )
{
    if (str == "DEC")           return CommandClass::DEC;
    if (str == "ASS")           return CommandClass::ASS;
    if (str == "DEC_ASS")       return CommandClass::DEC_ASS;
    if (str == "FN_BEG")        return CommandClass::FN_BEG;
    if (str == "FN_END")        return CommandClass::FN_END;
    if (str == "WH_BEG")        return CommandClass::WH_BEG;
    if (str == "WH_END")        return CommandClass::WH_END;
    if (str == "FR_BEG")        return CommandClass::FR_BEG;
    if (str == "FR_END")        return CommandClass::FR_END;
    if (str == "IF_BEG")        return CommandClass::IF_BEG;
    if (str == "IF_END")        return CommandClass::IF_END;
    if (str == "RETURN")        return CommandClass::RETURN;

    return CommandClass::NONE;
}


llcl::Type
llcl::type_fromstr( std::string str )
{
    if (str == "u32")   return Type::u32;
    if (str == "i32")   return Type::i32;
    if (str == "f32")   return Type::f32;

    if (str == "u64")   return Type::u64;
    if (str == "i64")   return Type::i64;
    if (str == "f64")   return Type::f64;

    return Type::u00;
}


void llcl::print_tree( std::string prefix, Node *node, bool is_left )
{
    if (node == nullptr)
    {
        std::cout << prefix << (is_left ? "├─ " : "└─ " ) << std::endl;
        return;
    }

    std::cout << prefix;
    std::cout << (is_left ? "├─ " : "└─ " );
    // std::cout << "< " << node->m_symbol.m_class << " : " << node->m_symbol.m_value << " >\n";
    std::cout << "<" << node->m_symbol.m_class << ": " << node->m_symbol.m_value << ">\n";

    llcl::print_tree( prefix + (is_left ? "│  " : "   "), node->right,  true);
    llcl::print_tree( prefix + (is_left ? "│  " : "   "), node->left, false);
}

