#pragma once

#include "../common/common.h"


namespace llcl
{
    struct Variable;
    struct Function;
}


struct llcl::Variable
{
    llcl::Type  m_type;
    size_t      m_byte_offset = 0;

    Variable( ): m_type(llcl::Type::u00) {  };
    Variable( llcl::Type datatype ): m_type(datatype) {  };
};


struct llcl::Function
{
    size_t                      m_num_params;
    size_t                      bytes_required = 8;

    // The first m_num_params variables are the function parameters
    std::map<std::string, Variable>     m_variables;
    std::vector<llcl::Command>          m_commands;


    void    add_param( llcl::Type datatype, std::string name );

    // Store command for later NASM generation
    void    push_command( llcl::Command command )
    {
        m_commands.push_back(command);
    }
};


