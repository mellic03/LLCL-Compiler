#include "parser.h"


void
llcl::Parser::write_parse_tree( ofstream &stream, llcl::Node *node, llcl::Function &function )
{
    // Base case, subject or literal
    if (node->left == nullptr && node->right == nullptr)
    {
        std::string value = "";

        if (node->m_symbol.m_class == SymbolClass::LITERAL)
            value = node->m_symbol.m_value;
        else if (node->m_symbol.m_class == SymbolClass::SUBJECT)
            value = "[rbp + " + std::to_string(function.m_variables[node->m_symbol.m_value].m_byte_offset) + "]";
        else
        {
            std::cout << "llcl::Parser::write_parse_tree():  THIS SHOULD NOT EXECUTE!!!\n";
            exit(1);
        }

        stream << "push qword " + value + "\n";

        return;
    }

    write_parse_tree(stream, node->left, function);
    write_parse_tree(stream, node->right, function);

    stream
        << "pop rdx\n"
        << "pop rax\n";

    if (node->m_symbol.m_value == "+")
    {
        stream
            << "add rax, rdx\n";
    }

    else if (node->m_symbol.m_value == "-")
    {
        stream
            << "sub rax, rdx\n";
    }

    else if (node->m_symbol.m_value == "*")
    {
        stream
            << "mul rdx\n";
    }

    else if (node->m_symbol.m_value == "/")
    {
        stream
            << "div rdx\n";
    }

    stream << "push rax\n";

}



void
llcl::Parser::write_function( ofstream &stream, std::string name, Function &function )
{
    stream << name + ":\n";
    stream.right();

    // Prologue
    stream
        << ";---------------------; Function prologue\n"
        << "push qword rbp\n"
        << "mov rbp, rsp\n"
        << "sub rsp, " << function.bytes_required << "\n"
        << ";---------------------------------------;\n";

    // Function body
    stream
        << ";---------------------; Function body\n";
    
    for (Command &command: function.m_commands)
    {

        // Evaluate expression to right of "="
        if (command.m_class == CommandClass::DEC_ASS)
        {
            // i64 x = 50
            std::string var_name = command.m_symbols[1].m_value;
            Variable &var = function.m_variables[var_name];

            auto expr = std::vector<Symbol>(command.m_symbols.begin()+3, command.m_symbols.end());

            llcl::Node *node = gen_parse_tree(expr);
            write_parse_tree(stream, node, function);

            stream
                << "pop qword rax\n"
                << "mov qword [rbp + " + std::to_string(var.m_byte_offset) + "], rax\n";
        }

        else if (command.m_class == CommandClass::ASS)
        {
            // x = 50
            std::string var_name = command.m_symbols[0].m_value;
            Variable &var = function.m_variables[var_name];

            auto expr = std::vector<Symbol>(command.m_symbols.begin()+2, command.m_symbols.end());

            llcl::Node *node = gen_parse_tree(expr);
            write_parse_tree(stream, node, function);

            stream
                << "pop qword rax\n"
                << "mov qword [rbp + " + std::to_string(var.m_byte_offset) + "], rax\n";
        }

        // Evaluate expression to right of "return"
        if (command.m_class == CommandClass::RETURN)
        {
            auto expr = std::vector<Symbol>(command.m_symbols.begin()+1, command.m_symbols.end());

            llcl::Node *node = gen_parse_tree(expr);
            write_parse_tree(stream, node, function);

            stream
                << "pop rax\n";
        }
    }

    stream
        << ";-----------------------------------;\n";


    // Epilogue
    stream
        << ";---------------------; Function epilogue\n"
        << "mov rsp, rbp\n"
        << "pop rbp\n"
        << "ret\n"
        << ";---------------------------------------;\n";

    stream.left();
}


void
llcl::Parser::writeNASM( ofstream &stream )
{
    // includes -------------------------------------
    stream
        << "%include \"linux64.inc\""
        << "\n\n";
    // ----------------------------------------------


    // write .data ----------------------------------
    stream << "section .data\n";
    stream.right();

    stream << "\n";
   
    stream.left();
    // ----------------------------------------------


    // write .bss -----------------------------------
    stream << "section .bss\n";
    stream.right();
    
    stream << "\n";
    
    stream.left();
    // ----------------------------------------------


    // write .text ----------------------------------
    stream << "section .text\n";
    stream.right();
    
    stream << "global _start\n";
    stream << "\n";

    stream.left();
    // ----------------------------------------------


    // Write functions ------------------------------
    for (auto &[name, function]: m_functions)
    {
        write_function(stream, name, function);
    }
    // ----------------------------------------------


    // write _start ---------------------------------
    stream << "_start:\n";
    stream.right();
    
    stream
        << "push 1\n"
        << "push 0\n"
        << "call LLCL_FN_main\n"
        << "exit rax\n\n";
    
    stream.left();
    // ----------------------------------------------

}



