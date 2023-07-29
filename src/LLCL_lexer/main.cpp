
#include "lexer.h"


int main(int argc, char **argv)
{
    llcl::Lexer lexer;

    std::ifstream instream(argv[1]);
    std::ofstream outstream(argv[2]);
    std::string line;

    while (std::getline(instream, line))
    {
        outstream << lexer.feedline(line) << "\n";
    }

    instream.close();
    outstream.close();

    return 0;
}

