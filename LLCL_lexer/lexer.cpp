#include "lexer.h"

std::ostream &operator << (std::ostream &os, SLTokenType token_type)
{
    switch (token_type)
    {
        case SLTokenType::LITERAL:      os << "LITERAL";    break;
        case SLTokenType::DATATYPE:     os << "DATATYPE";   break;
        case SLTokenType::VAR_NAME:     os << "VAR_NAME";   break;
        case SLTokenType::FN_NAME:      os << "FN_NAME";    break;
        case SLTokenType::FN_BEG:       os << "FN_BEG";     break;
        case SLTokenType::FN_END:       os << "FN_END";     break;
        case SLTokenType::RETURN:       os << "RETURN";     break;
        case SLTokenType::IF_BEGIN:     os << "IF_BEG";     break;
        case SLTokenType::IF_END:       os << "IF_END";     break;
        case SLTokenType::OPERATOR:     os << "OPERATOR";   break;
    }

    return os;
}


SLTokenType SLTokenType_fromstr( std::string token_str )
{
    if (token_str == "LITERAL")     return SLTokenType::LITERAL;
    if (token_str == "DATATYPE")    return SLTokenType::DATATYPE;
    if (token_str == "VAR_NAME")    return SLTokenType::VAR_NAME;
    if (token_str == "FN_NAME")     return SLTokenType::FN_NAME;
    if (token_str == "FN_BEG")      return SLTokenType::FN_BEG;
    if (token_str == "FN_END")      return SLTokenType::FN_END;
    if (token_str == "RETURN")      return SLTokenType::RETURN;
    if (token_str == "IF_BEGIN")    return SLTokenType::IF_BEGIN;
    if (token_str == "IF_END")      return SLTokenType::IF_END;
    if (token_str == "OPERATOR")    return SLTokenType::OPERATOR;

    return SLTokenType::NONE;
}

