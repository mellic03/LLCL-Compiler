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


std::ostream &operator << (std::ostream &os, llcl::SymbolClass s_class)
{
    switch (s_class)
    {
        case llcl::SymbolClass::LITERAL:    os << "LITERAL";    break;
        case llcl::SymbolClass::SUBJECT:    os << "SUBJECT";    break;
        case llcl::SymbolClass::OPERATOR:   os << "OPERATOR";   break;
        case llcl::SymbolClass::FN_BEG:     os << "FN_BEG";     break;
        case llcl::SymbolClass::FN_END:     os << "FN_END";     break;
        case llcl::SymbolClass::WH_BEG:     os << "WH_BEG";     break;
        case llcl::SymbolClass::WH_END:     os << "WH_END";     break;
        case llcl::SymbolClass::IF_BEG:     os << "IF_BEG";     break;
        case llcl::SymbolClass::IF_END:     os << "IF_END";     break;
        case llcl::SymbolClass::NONE:       os << "NONE";       break;
    }

    return os;
}


llcl::SymbolClass llcl::symbolclass_fromstr( std::string str )
{
    if (str == "LITERAL")       return SymbolClass::LITERAL;
    if (str == "SUBJECT")       return SymbolClass::SUBJECT;
    if (str == "OPERATOR")      return SymbolClass::OPERATOR;
    if (str == "FN_BEG")        return SymbolClass::FN_BEG;
    if (str == "FN_END")        return SymbolClass::FN_END;
    if (str == "WH_BEG")        return SymbolClass::WH_BEG;
    if (str == "WH_END")        return SymbolClass::WH_END;
    if (str == "IF_BEG")        return SymbolClass::IF_BEG;
    if (str == "IF_END")        return SymbolClass::IF_END;

    return SymbolClass::NONE;
}
