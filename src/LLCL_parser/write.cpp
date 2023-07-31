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

        stream << "push  qword   " + value + "\n";

        return;
    }

    write_parse_tree(stream, node->left, function);
    write_parse_tree(stream, node->right, function);

    stream
        << "pop   r8\n"
        << "pop   rax\n";

    if (node->m_symbol.m_value == "+")
    {
        stream
            << "add   rax,   r8\n";
    }

    else if (node->m_symbol.m_value == "-")
    {
        stream
            << "sub   rax,   r8\n";
    }

    else if (node->m_symbol.m_value == "*")
    {
        stream
            << "mul   r8\n";
    }

    else if (node->m_symbol.m_value == "/")
    {
        stream
            << "cqo\n"
            << "div   r8\n";
    }

    stream << "push  rax\n";

}


static int loop_nesting_level = 0;

void
llcl::Parser::write_for_begin( ofstream &stream, Function &function, Command &command )
{
    if (loop_nesting_level == 0)
    {
        function.m_loop_counter += 1;
    }


    std::string loopcount = std::to_string(function.m_loop_counter);
    std::string nesting   = std::to_string(loop_nesting_level);
    std::string loop_id = loopcount + "_" + nesting;

    auto &symbols = command.m_symbols;

    auto loop_begin = symbols[1].m_value;
    auto loop_end   = symbols[2].m_value;

    stream << "\n";

    // Begin loop ---------------------------
    stream
        << "push  r9   ; loop counter\n"
        << "push  rcx  ; end value\n"
        << "mov   r9,  " + loop_begin + "\n"
        << "mov   rcx, " + loop_end + "\n"
        << "cmp   r9,   rcx\n"
        << "jge   .LLCL_LOOP_END_" + loop_id + "\n"
        << ".LLCL_LOOP_BEGIN_"     + loop_id + ":\n";
    // --------------------------------------

    loop_nesting_level += 1;

    stream.right();
}


void
llcl::Parser::write_for_end( ofstream &stream, Function &function, Command &command )
{
    loop_nesting_level -= 1;

    std::string loopcount = std::to_string(function.m_loop_counter);
    std::string nesting   = std::to_string(loop_nesting_level);
    std::string loop_id = loopcount + "_" + nesting;

    auto &symbols = command.m_symbols;

    // Increment loop counter ---------------
    stream
        << "add   r9,  1  ; increment loop counter\n"
        << "cmp   r9,  rcx\n"
        << "jge   .LLCL_LOOP_END_"   + loop_id + "\n"
        << "jmp   .LLCL_LOOP_BEGIN_" + loop_id + "\n";
    // --------------------------------------

    // End loop -----------------------------
    stream.left();
    stream
        << ".LLCL_LOOP_END_" + loop_id + ":\n"
        << "\tpop   rcx\n"
        << "\tpop   r9\n";
    // --------------------------------------

}


void
llcl::Parser::write_dec_ass_fn_call( ofstream &stream, int command_idx, Function &function )
{
    Command &command = function.m_commands[command_idx];
    auto &symbols = command.m_symbols;

    // Determine input parameters, push them to stack
    for (int i=5; i<symbols.size(); i++)
    {
        auto &var = function.m_variables[symbols[i].m_value];

        if (symbols[i].m_class == SymbolClass::LITERAL)
            stream << "push  qword   " + symbols[i].m_value + "\n";

        else if (symbols[i].m_class == SymbolClass::SUBJECT)
            stream << "push  qword   [rbp + " + std::to_string(var.m_byte_offset) + "]\n";
    }

    std::string dest_addr = std::to_string(function.m_variables[symbols[1].m_value].m_byte_offset);

    stream
        << "call  LLCL_FN_" + symbols[3].m_value + "\n"
        << "mov   qword   [rbp + " + dest_addr  + "],   rax\n";
}



void
llcl::Parser::write_dec_ass( ofstream &stream, int command_idx, Function &function )
{
    Command &command = function.m_commands[command_idx];

    // i64 x = function_name()
    if (command.m_symbols.size() >= 4)
    if (m_function_names.find(command.m_symbols[3].m_value) != m_function_names.end())
    {
        write_dec_ass_fn_call(stream, command_idx, function);
        return;
    }

    std::string var_name = command.m_symbols[1].m_value;
    Variable &var = function.m_variables[var_name];

    auto expr = std::vector<Symbol>(command.m_symbols.begin()+3, command.m_symbols.end());

    llcl::Node *node = gen_parse_tree(expr);
    write_parse_tree(stream, node, function);

    stream
        << "pop   qword   rax\n"
        << "mov   qword   [rbp + " + std::to_string(var.m_byte_offset) + "], rax\n";
}


void
llcl::Parser::write_function( ofstream &stream, std::string name, Function &function )
{
    stream << name + ":\n";
    stream.right();

    // Prologue
    stream
        << ";---------------------; Function prologue\n"
        << "push  qword  rbp\n"
        << "mov   rbp,   rsp\n"
        << "sub   rsp,   " + std::to_string(function.bytes_required) << "\n"
        << ";---------------------------------------;\n";

    // Function body
    stream
        << ";---------------------; Function body\n";
    
    for (int i=0; i<function.m_commands.size(); i++)
    {
        Command &command = function.m_commands[i];

        // Evaluate expression to right of "="
        if (command.m_class == CommandClass::DEC_ASS)
        {
            write_dec_ass(stream, i, function);
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
                << "pop   qword   rax\n"
                << "mov   qword   [rbp + " + std::to_string(var.m_byte_offset) + "], rax\n";
        }

        else if (command.m_class == CommandClass::FR_BEG)
        {
            write_for_begin(stream, function, command);
        }

        else if (command.m_class == CommandClass::FR_END)
        {
            write_for_end(stream, function, command);
        }

        // Evaluate expression to right of "return"
        if (command.m_class == CommandClass::RETURN)
        {
            auto expr = std::vector<Symbol>(command.m_symbols.begin()+1, command.m_symbols.end());

            llcl::Node *node = gen_parse_tree(expr);
            write_parse_tree(stream, node, function);

            stream
                << "pop   rax\n";
        }
    }

    stream
        << ";-----------------------------------;\n";


    // Epilogue
    stream
        << ";---------------------; Function epilogue\n"
        << "mov   rsp,   rbp\n"
        << "pop   rbp\n"
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
        << "push  1\n"
        << "push  0\n"
        << "call  LLCL_FN_main\n"
        << "exit  rax\n\n";
    
    stream.left();
    // ----------------------------------------------

}



