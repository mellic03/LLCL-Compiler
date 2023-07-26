#include "NASM_Interface.h"


std::string getIndent(int n)
{
    std::string str = "";
    for (int i=0; i<n; i++)
        str += "\t";
    return str;
}


NASM_str
NASM_Interface::add_const_str( std::string name, std::string value )
{
    size_t num_bytes = value.length();

    for (size_t i=0; i<value.length(); i++)
    {
        if (value[i] == '\n')
        {
            value.erase(i);
            value.insert(i, "\\n");
            i += 1;
        }
    }

    std::string nasm_name = SL_var_prefix + name;
    std::string nasm_line = nasm_name + " db " + "`" + value + "`" + "\n";

    m_stream << "\t" << nasm_line ;
    NASM_str const_str = { name, num_bytes };

    return const_str;
}


NASM_str
NASM_Interface::add_dyn_str( std::string name, size_t num_bytes )
{
    std::string nasm_name = SL_var_prefix + name;
    std::string nasm_line = nasm_name + " resb " + std::to_string(num_bytes);
    m_stream << "\t" << nasm_line << "\n";

    NASM_str dyn = { name, num_bytes };

    return dyn;
}


NASM_bytes
NASM_Interface::i32_declaration( std::string name, size_t num_bytes )
{
    std::string nasm_name = SL_var_prefix + SL_i32_prefix + name;
    std::string nasm_line = nasm_name + " resb " + std::to_string(num_bytes);
    m_stream << "\t" << nasm_line << "\n";

    NASM_bytes dyn = { name, num_bytes };

    return dyn;
}


void
NASM_Interface::i32_assignment( const NASM_bytes &bytes, uint32_t data )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov dword [" << SL_var_prefix << SL_i32_prefix << bytes.name << "], " << data << "\n";
}


void
NASM_Interface::localvar_signed_add( NASM_localvar &a, NASM_localvar &b )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov rax, qword [rbp + " << a.offset << "]\n"
        << ind << "add rax, qword [rbp + " << b.offset << "]\n"
        << ind << "mov qword [rbp + " << a.offset << "], rax\n";
}


void
NASM_Interface::localvar_signed_sub( NASM_localvar &a, NASM_localvar &b )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov rax, qword [rbp + " << a.offset << "]\n"
        << ind << "sub rax, qword [rbp + " << b.offset << "]\n"
        << ind << "mov qword [rbp + " << a.offset << "], rax\n";
}


void
NASM_Interface::write_defines()
{
    #ifdef __unix__
    std::vector<std::string> defines
    {
        "\%include \"linux64.inc\"",
        "",
        "\%define newline 10",
        "",
        "\%macro exit 1\n"
        "\tmov rdi, %1\n"
        "\tmov rax, SYS_EXIT\n"
        "\tsyscall\n"
        "\%endmacro",
        "",
    };
    #elif defined(_WIN32) || defined(WIN32)
    std::vector<std::string> defines
    {
        "\%define newline 10",
        "\%define sys_read 0",
        "\%define sys_write 1",
        "\%define SYS_EXIT 60"
    };
    #endif

    for (auto &define: defines)
    {
        m_stream << define << "\n";
    }
}


void
NASM_Interface::data_begin()
{
    m_stream
        << "\n"
        << getIndent(m_indentation) << "section .data\n";
    m_indentation += 1;
}

void
NASM_Interface::data_end()
{
    m_stream << "\n";
    m_indentation -= 1;
}


void
NASM_Interface::bss_begin()
{
    m_stream
        << "\n"
        << getIndent(m_indentation) << "section .bss\n";
    m_indentation += 1;
}

void
NASM_Interface::bss_end()
{
    m_stream << "\n";
    m_indentation -= 1;
}


void
NASM_Interface::text_begin()
{
    m_stream << "\nsection .text\n";
    m_stream << "\tglobal _start\n";
    m_indentation += 1;
}


void
NASM_Interface::text_end()
{
    m_stream << "\n";
    m_indentation -= 1;
}


NASM_fn
NASM_Interface::function_begin( std::string name )
{
    m_cond_count = 0;

    auto ind = getIndent(m_indentation);

    m_stream << "\n";
    m_stream << ind << SL_fn_prefix << name << ":\n";

    m_indentation += 1;

    return { name, 0 };
}


NASM_localvar
NASM_Interface::function_push_localvar( NASM_fn &fn, size_t num_bytes )
{
    fn.num_bytes_prologue += num_bytes;

    if (fn.local_variables.size() > 0)
        fn.local_variables.push_back(  { num_bytes + fn.local_variables.back().offset }  );
    else
        fn.local_variables.push_back(  { num_bytes + 8 }  );

    return fn.local_variables.back();
}


void
NASM_Interface::function_assign_localvar( NASM_fn &fn, NASM_localvar &var, std::string value )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov qword [rbp + " << var.offset << "], " << value << "\n";
}


void
NASM_Interface::function_write_prologue( const NASM_fn &fn )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << ";-----------------------------------; function prologue\n"
        << ind << "push rbp\n"
        << ind << "mov rbp, rsp\n"
        << ind << "sub rsp, " << fn.num_bytes_prologue << "\n"
        << ind << ";-----------------------------------------------------;\n";
}


void
NASM_Interface::function_return( const NASM_fn &fn, const NASM_bytes &bytes )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << ";-----------------------------------; function epilogue\n"
        << ind << "mov " << SL_NASM_return_register << ", ["
               << SL_var_prefix << SL_i32_prefix << bytes.name
        << ind << "]\t; return value\n";
}


void
NASM_Interface::function_return( const NASM_fn &fn, const NASM_localvar &var )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << ";-----------------------------------; function epilogue\n"
        << ind << "mov " << SL_NASM_return_register << ", [rbp + " << var.offset << "]\t; return value\n"
        << ind << "mov rsp, rbp\n"
        << ind << "pop rbp\n";
}


void
NASM_Interface::function_return( const NASM_fn &fn, std::string value )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << ";-----------------------------------; function epilogue\n"
        << ind << "mov " << SL_NASM_return_register << ", " << value << "\t; return value\n"
        << ind << "mov rsp, rbp\n"
        << ind << "pop rbp\n";
}

void
NASM_Interface::function_end()
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "ret\n"
        << ind << ";-----------------------------------------------------;\n";
   
    m_indentation -= 1;
}


void
NASM_Interface::function_call( const NASM_fn &fn )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "call " << SL_fn_prefix << fn.name << "\n";
}


void
NASM_Interface::function_call_pushparam( const NASM_fn &fn, NASM_localvar var )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << "push qword " << "rbp[" << var.offset << "]";
}


void
NASM_Interface::function_call_pushparam( const NASM_fn &fn, int literal )
{

}





NASM_conditional
NASM_Interface::conditional_begin( NASM_comparator cmp, const NASM_bytes &left, const NASM_bytes &right )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov eax, " << "[" << SL_var_prefix << SL_i32_prefix << left.name  << "]\n"
        << ind << "cmp eax, " << "[" << SL_var_prefix << SL_i32_prefix << right.name << "]\n";

    std::string cond_str = std::to_string(m_cond_count);

    NASM_conditional cond = {
        cmp,
        ".SL_COND" + cond_str + "_IF",
        ".SL_COND" + cond_str + "_ELSE",
        ".SL_COND" + cond_str + "_END"
    };

    m_cond_count += 1;

    switch (cmp)
    {
        case NASM_comparator::LESS_THAN:
            m_stream << ind << "jl " << cond.if_block << "\n";
            m_stream << ind << "jg " << cond.else_block << "\n";
            m_stream << ind << "je " << cond.else_block << "\n";
            break;

        case NASM_comparator::GREATER_THAN:
            m_stream << ind << "jg " << cond.if_block << "\n";
            m_stream << ind << "jl " << cond.else_block << "\n";
            m_stream << ind << "je " << cond.else_block << "\n";
            break;

        case NASM_comparator::EQUAL:
            m_stream << ind << "je " << cond.if_block << "\n";
            m_stream << ind << "jl " << cond.else_block << "\n";
            m_stream << ind << "jg " << cond.else_block << "\n";
            break;
    }

    return cond;
}

NASM_conditional
NASM_Interface::conditional_begin( NASM_comparator cmp, const NASM_localvar &left, const NASM_localvar &right )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "mov rax, " << "[rbp + " << left.offset  << "]\n"
        << ind << "cmp rax, " << "[rbp + " << right.offset << "]\n";

    std::string cond_str = std::to_string(m_cond_count);

    NASM_conditional cond = {
        cmp,
        ".SL_COND" + cond_str + "_IF",
        ".SL_COND" + cond_str + "_ELSE",
        ".SL_COND" + cond_str + "_END"
    };

    m_cond_count += 1;

    switch (cmp)
    {
        case NASM_comparator::LESS_THAN:
            m_stream << ind << "jl " << cond.if_block << "\n";
            m_stream << ind << "jg " << cond.else_block << "\n";
            m_stream << ind << "je " << cond.else_block << "\n";
            break;

        case NASM_comparator::GREATER_THAN:
            m_stream << ind << "jg " << cond.if_block << "\n";
            m_stream << ind << "jl " << cond.else_block << "\n";
            m_stream << ind << "je " << cond.else_block << "\n";
            break;

        case NASM_comparator::EQUAL:
            m_stream << ind << "je " << cond.if_block << "\n";
            m_stream << ind << "jl " << cond.else_block << "\n";
            m_stream << ind << "jg " << cond.else_block << "\n";
            break;
    }

    return cond;
}


void
NASM_Interface::conditional_begin_onif( NASM_conditional &cond )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << cond.if_block << ":\n";
    
    m_indentation += 1;
}

void
NASM_Interface::conditional_end_onif( NASM_conditional &cond )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "jmp " << cond.end_block << "\n";

    m_indentation -= 1;
}

void
NASM_Interface::conditional_begin_onelse( NASM_conditional &cond )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << cond.else_block << ":\n";

    m_indentation += 1;
}

void
NASM_Interface::conditional_end_onelse( NASM_conditional &cond )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << "jmp " << cond.end_block << "\n";

    m_indentation -= 1;
}


void
NASM_Interface::conditional_end( NASM_conditional cond )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << cond.end_block << ":\n";
}


void
NASM_Interface::start_begin()
{
    m_stream
        << "\n"
        << getIndent(m_indentation) << "_start:"
        << "\n";

    m_indentation += 1;
}

void
NASM_Interface::start_end()
{
    m_indentation -= 1;
}


void
NASM_Interface::NASM_embed( std::string line )
{
    auto ind = getIndent(m_indentation);
    m_stream
        << ind << line << "\n";
}


void
NASM_Interface::print_str(NASM_str str)
{
    auto ind = getIndent(m_indentation);

    m_stream
        << ind << "mov rax, SYS_WRITE" << "\n"
        << ind << "mov rdi, STDOUT" << "\n"
        << ind << "mov rsi, " << SL_var_prefix << str.name << "\n"
        << ind << "mov rdx, " << str.num_bytes << "\n"
        << ind << "syscall\n";
}


void
NASM_Interface::scan_str(NASM_str str)
{
    auto ind = getIndent(m_indentation);

    m_stream
        << ind << "mov rax, SYS_READ" << "\n"
        << ind << "mov rdi, STDIN" << "\n"
        << ind << "mov rsi, " << SL_var_prefix << str.name << "\n"
        << ind << "mov rdx, " << str.num_bytes << "\n"
        << ind << "syscall\n";
}
