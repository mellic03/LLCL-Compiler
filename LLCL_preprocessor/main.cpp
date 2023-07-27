#include "../common/common.h"
#include "../LLCL_lexer/lexer.h"



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

    while (std::getline(instream, line))
    {
        // Remove comments
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos)
        {
            line = line.substr(0, comment_pos);
        }

        // Macros
        size_t define_pos = line.find("#define");
        if (define_pos != std::string::npos)
        {
            // Remove "#define"
            line = line.substr(8);
            std::string name = line.substr(0, line.find("("));
            line = line.substr(line.find("("));

            auto tokens = llcltok::seppar(line);
            for (auto &token: tokens)
            {
                std::cout << "\"" << token << "\"\n";
            }

            macros[name] = "MEMBLOCK";
            line = "";
        }

        for (auto &[name, value]: macros)
        {
            size_t macro_pos = line.find(name);
            if (macro_pos != std::string::npos)
            {
                line.erase(macro_pos, name.length());
                line.insert(macro_pos, value);
            } 
        }

        outstream << line << "\n";
    }

    instream.close();
    outstream.close();

    return 0;
}

