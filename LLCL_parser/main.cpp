#include "parser.h"



void parse_function( NASM_Interface &nasm, std::vector<std::vector<SLToken>> &function_tokens )
{
    NASM_fn nfunction = nasm.function_begin(function_tokens[0][1].value);
    std::map<std::string, NASM_localvar> local_variables;

    size_t num_bytes = 0;

    // Determine how much space to allocate for local variables
    for (auto &tokens: function_tokens)
    {
        switch (tokens[0].type)
        {
            case SLTokenType::DATATYPE:
            {
                num_bytes += 8;
                local_variables[tokens[1].value] = nasm.function_push_localvar(nfunction, 8);
                break;
            }
        }
    }
    nasm.function_write_prologue(nfunction);


    // Do function logic
    for (auto &tokens: function_tokens)
    {
        switch (tokens[0].type)
        {
            case SLTokenType::DATATYPE:
            {
                // if declaration only
                if (tokens.size() == 2)
                {
                    local_variables[tokens[1].value] = nasm.function_push_localvar(nfunction, 8);

                }

                // if declaration + assignment
                else
                {
                    NASM_localvar var = nasm.function_push_localvar(nfunction, 8);
                    nasm.function_assign_localvar(nfunction, var, tokens[3].value);
                    local_variables[tokens[1].value] = var;
                }

                break;
            }

            case SLTokenType::RETURN:
            {
                if (tokens[1].type == SLTokenType::LITERAL)
                    nasm.function_return(nfunction, tokens[1].value);
                else if (tokens[1].type == SLTokenType::VAR_NAME)
                    nasm.function_return(nfunction, local_variables[tokens[1].value]);
                break;
            }
        }
    }


    nasm.function_end();
}


int main( int argc, char **argv )
{
    if (argc != 3)
    {
        std::cout << "NOPE!\n";
        return 1;
    }

    SLParser parser;
    parser.loadTokens(argv[1]);
    NASM_Interface nasm(argv[2]);


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

    NASM_fn current_fn;

    for (int i=0; i<parser.m_tokens.size(); i++)
    {
        auto &tokens = parser.m_tokens[i];

        // determine which type of command this line is
        switch (tokens[0].type)
        {
            // If function begin, collect all lines until function end and parse
            case SLTokenType::FN_BEG:
            {
                std::vector<std::vector<SLToken>> function_tokens;

                while (parser.m_tokens[i][0].type != SLTokenType::FN_END)
                {
                    function_tokens.push_back(parser.m_tokens[i]);
                    i += 1;
                }
                function_tokens.push_back(parser.m_tokens[i]);

                parse_function(nasm, function_tokens);
                break;
            }

        }

    }


    nasm.start_begin();
        nasm.NASM_embed("push 50");
        nasm.NASM_embed("push 30");
        nasm.NASM_embed("call SL_FN_main");
        nasm.NASM_embed("exit rax");
    nasm.start_end();


    return 0;
}