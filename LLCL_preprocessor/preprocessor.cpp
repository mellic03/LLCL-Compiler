#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

std::set<std::string> macro_names;


int main( int argc, char **argv )
{
    if (argc != 3)
    {
        return 1;
    }

    std::ifstream instream(argv[1]);
    std::ifstream outstream(argv[2]);
    std::string line;

    while (std::getline(instream, line))
    {
        
    }



    return 0;
}

