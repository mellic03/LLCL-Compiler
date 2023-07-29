#pragma once

#include "../common/common.h"
#include "../LLCL_lexer/lexer.h"



struct LLCL_param_macro
{
    std::string  m_value;
    std::string  m_param_name;
    size_t       m_param_pos;

    std::string  genStr(std::string param)
    {
        std::string str = m_value;
        std::cout << "PREERASED: " << str << "\n";
        str.erase(m_param_pos, m_param_name.size());
        std::cout << "ERASED: " << str << "\n";
        str.insert(m_param_pos, param);
        return str;
    };
};


class LLCLPP
{
private:
    std::map<std::string, std::string> m_basic_macros;
    std::map<std::string, LLCL_param_macro> m_param_macros;

public:
                        LLCLPP() {  };

    std::string &       remove_comment( std::string &line );

    bool                is_basic_macro_declaration( std::string line );
    void                load_basic_macro_declaration( std::string &line );

    bool                is_param_macro_declaration( std::string line );
    void                load_param_macro_declaration( std::string &line );


    bool                find_basic_macro_reference( std::string line, std::string &name, size_t &pos );
    void                replace_basic_macro_reference( std::string &line, std::string name, size_t pos );

    bool                find_param_macro_reference( std::string line, std::string &name, size_t &pos );
    void                replace_param_macro_reference( std::string &line, std::string name, size_t pos );

};
