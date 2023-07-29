#include "preprocessor.h"



std::string concatTokens( const std::vector<std::string> &tokens, size_t a, size_t b )
{
    std::string str = "";
    for (size_t i=a; i<=b; i++)
    {
        str += tokens[i];
    }
    return str;
}


std::map<std::string, std::string> macros;



int main( int argc, char **argv )
{
    if (argc != 3)
    {
        return 1;
    }

    std::ifstream instream(argv[1]);
    std::ofstream outstream(argv[2]);
    std::string line;

    LLCLPP llclpp;

    while (std::getline(instream, line))
    {
        line = llclpp.remove_comment(line);

        if (llclpp.is_param_macro_declaration(line))
            llclpp.load_param_macro_declaration(line);

        else if (llclpp.is_basic_macro_declaration(line))
            llclpp.load_basic_macro_declaration(line);

        std::string macro_name;
        size_t macro_pos;

        while (llclpp.find_basic_macro_reference(line, macro_name, macro_pos))
            llclpp.replace_basic_macro_reference(line, macro_name, macro_pos);

        while (llclpp.find_param_macro_reference(line, macro_name, macro_pos))
            llclpp.replace_param_macro_reference(line, macro_name, macro_pos);


        std::cout << line << "\n";
        outstream << line << "\n";
    }

    instream.close();
    outstream.close();

    return 0;
}

