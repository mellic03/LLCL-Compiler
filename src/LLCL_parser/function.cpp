#include "function.h"




void
llcl::Function::add_param( llcl::Type datatype, std::string name )
{
    m_variables[name] = Variable(datatype);
}
