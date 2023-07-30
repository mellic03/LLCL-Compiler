
#include "LLCL_lexer/lexer.h"
#include "LLCL_parser/parser.h"


int main(int argc, char **argv)
{
    llcl::Lexer lexer;

    std::ifstream instream(argv[1]);
    std::string line;

    while (std::getline(instream, line))
    {
        lexer.feedline(line);
    }
    instream.close();



    llcl::Parser parser;
    llcl::Command command;

    while (lexer.getcommand(command))
    {
        std::cout << command.m_class << "\t";
        for (auto &symbol: command.m_symbols)
            std::cout << "<" << symbol.m_class << ": " << symbol.m_value << ">  ";
        std::cout << "\n";

        parser.feedcommand(command);
    }


    return 0;
}

