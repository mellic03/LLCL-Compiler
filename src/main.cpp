
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
        // std::cout << command.m_class << "\t";
        // for (auto &symbol: command.m_symbols)
        //     std::cout << "<" << symbol.m_class << ": " << symbol.m_value << ">  ";
        // std::cout << "\n";

        parser.feedcommand(command);
    }

    llcl::ofstream outstream(argv[2]);
    parser.writeNASM(outstream);
    outstream.close();


    std::vector<llcl::Symbol> symbols;

    // (a + b) * 20 - (10 / 2)

    symbols.push_back({llcl::SymbolClass::LPAR,     "("});
    symbols.push_back({llcl::SymbolClass::LPAR,     "("});
    symbols.push_back({llcl::SymbolClass::SUBJECT,  "a"});
    symbols.push_back({llcl::SymbolClass::OPERATOR, "+"});
    symbols.push_back({llcl::SymbolClass::SUBJECT,  "b"});
    symbols.push_back({llcl::SymbolClass::RPAR,     ")"});
    symbols.push_back({llcl::SymbolClass::RPAR,     ")"});

    symbols.push_back({llcl::SymbolClass::OPERATOR, "*"});

    symbols.push_back({llcl::SymbolClass::LITERAL,  "20"});

    llcl::Node *node = parser.gen_parse_tree(symbols);
    llcl::print_tree("", node, false);

    return 0;
}

