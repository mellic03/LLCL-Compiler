#include "NASM_Interface.h"


// std::string &stripws( std::string &str )
// {
//     int idx = 0;
//     while (str[idx] == ' ' && str[idx] != '\0')
//         idx += 1;
//     str = str.substr(idx);

//     idx = str.length() - 1;
//     while (str[idx] == ' ' && str[idx] != '\0')
//         idx -= 1;
//     str = str.substr(0, idx+1);

//     return str;
// }


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        std::cout << "NOPE!\n";
        return 1;
    }

    NASM_Interface nasm(argv[1]);

    nasm.data_begin();
        NASM_str str = nasm.add_const_str("str", "Hello, dear reader.\n");
    nasm.data_end();


    nasm.bss_begin();
        NASM_str username = nasm.add_dyn_str("username", 16);
        nasm.NASM_embed( "SL_MEM_A resb 25 * 1000" );
        nasm.NASM_embed( "SL_MEM_B resb 25 * 1000" );
        nasm.NASM_embed( "SL_MEM_C resb 25 * 1000" );
        nasm.NASM_embed( "SL_MEM_D resb 25 * 1000" );
    nasm.bss_end();


    nasm.text_begin();
    nasm.text_end();


    NASM_fn fn_print_str = nasm.function_begin("print_str");
        nasm.function_write_prologue(fn_print_str);
        nasm.print_str(str);
        nasm.function_return(fn_print_str, 0);
    nasm.function_end();


    NASM_fn fn_main = nasm.function_begin("main");

        NASM_localvar param_b = nasm.function_push_localvar(fn_main, 8);
        NASM_localvar param_a = nasm.function_push_localvar(fn_main, 8);
        nasm.function_write_prologue(fn_main);

        nasm.localvar_signed_sub(param_a, param_b);
        nasm.function_call(fn_print_str);

        nasm.function_return(fn_main, param_a);
    nasm.function_end();


    nasm.start_begin();
        nasm.NASM_embed("push 50");
        nasm.NASM_embed("push 30");
        nasm.function_call(fn_main);
        nasm.NASM_embed("exit rax");
    nasm.start_end();


    return 0;
}

