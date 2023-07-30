#pragma once

#include <fstream>

namespace llcl { class ofstream; };

class llcl::ofstream
{
private:
    std::ofstream   m_outstream;
    int             m_indentation = 0;

    std::string     getindent( int ind )
    {
        std::string str = "";
        for (int i=0; i<ind; i++)
            str += "\t";
        return str;
    };

public:
    ofstream( std::string path ): m_outstream(path) {  };

    ofstream& operator<<(std::ostream& (*manipulator)(std::ostream&))
    {
        m_outstream << manipulator;
        return *this;
    }

    template <typename T>
    ofstream& operator<<(T rhs)
    {
        std::string ind = getindent(m_indentation);
        m_outstream << ind << rhs;
        return *this;
    };

    void close() { m_outstream.close(); };

    // Indent left
    void left() { m_indentation -= 1; };

    // Indent right
    void right() { m_indentation += 1; };

};

