
#include "lexer.h"


int main(int argc, char **argv)
{
    llcl::Lexer lexer;

    std::ifstream instream(argv[1]);
    std::ofstream outstream(argv[2]);
    std::string line;

    while (std::getline(instream, line))
    {
        lexer.feedline(line);
    }


    // outstream = std::ofstream("woop.asm");
    // llcl::Parser parser;
    
    // llcl::Command command;
    // while (lexer.nextcommand(command))
    // {
    //     parser.feedcommand(command);
    // }

    instream.close();
    outstream.close();

    return 0;
}

