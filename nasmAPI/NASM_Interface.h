#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <stdint.h>

#define SL_var_prefix "SL_VAR_"
#define SL_i32_prefix std::string("i32_")
#define SL_fn_prefix "SL_FN_"

#define SL_NASM_return_register "rax"


struct NASM_str
{
    std::string name = "";
    size_t num_bytes = 0;
};


struct NASM_bytes
{
    std::string name = "";
    size_t num_bytes = 0;
};



struct NASM_localvar
{
    size_t offset = 0;
};


struct NASM_fn
{
    std::string name = "";
    size_t num_bytes_prologue = 0;
    std::vector<NASM_localvar> local_variables;
};


enum class NASM_comparator
{
    LESS_THAN,
    GREATER_THAN,
    EQUAL
};

struct NASM_cmp
{

};

struct NASM_cond
{

};

struct NASM_conditional
{
    NASM_comparator comparator;
    std::string  if_block;
    std::string  else_block;
    std::string  end_block;
};



class NASM_Interface
{
private:
    std::ofstream       m_stream;
    int                 m_indentation = 0;
    int                 m_cond_count = 0;

    void                write_defines();


public:
                        NASM_Interface( std::string outputfile ): m_stream(outputfile) { write_defines(); };

    NASM_str            add_const_str( std::string name, std::string value );
    NASM_str            add_dyn_str( std::string name, size_t num_bytes );
    NASM_bytes          i32_declaration( std::string name, size_t num_bytes );
    void                i32_assignment( const NASM_bytes &bytes, uint32_t data );

                        /* add b to a */
    void                localvar_signed_add( NASM_localvar &a, NASM_localvar &b );
                        /* subtract b from a */
    void                localvar_signed_sub( NASM_localvar &a, NASM_localvar &b );

    void                data_begin();
    void                data_end();
    void                bss_begin();
    void                bss_end();
    void                text_begin();
    void                text_end();

    NASM_fn             function_begin( std::string name );

    /*
        The first paramter should be the first pushed and the last parameter
        should be the last pushed.

        Example:
            fn foo( i64 a, i64 b ): u00
                i64 c = 10
            endfn
    
            function_push_localvar(a);
            function_push_localvar(b);
            function_push_localvar(c);

    */
    NASM_localvar       function_push_localvar( NASM_fn &fn, size_t num_bytes );
    void                function_assign_localvar( NASM_fn &fn, NASM_localvar &var, std::string value );
    void                function_write_prologue( const NASM_fn &fn );
    void                function_return( const NASM_fn &fn, const NASM_bytes &bytes );
    void                function_return( const NASM_fn &fn, const NASM_localvar &var );
    void                function_return( const NASM_fn &fn, std::string value );
    void                function_end();
    void                function_call( const NASM_fn &fn );

    void                function_call_pushparam( const NASM_fn &fn, NASM_localvar );
    void                function_call_pushparam( const NASM_fn &fn, int literal );



    NASM_conditional    conditional_begin( NASM_comparator cmp, const NASM_bytes &left, const NASM_bytes &right );
    NASM_conditional    conditional_begin( NASM_comparator cmp, const NASM_localvar &left, const NASM_localvar &right );
    void                conditional_begin_onif( NASM_conditional &cond );
    void                conditional_end_onif( NASM_conditional &cond );
    void                conditional_begin_onelse( NASM_conditional &cond );
    void                conditional_end_onelse( NASM_conditional &cond );
    void                conditional_end( NASM_conditional conditional );

    void                start_begin();
    void                start_end();
    void                NASM_embed( std::string line );
    void                print_str(NASM_str str);
    void                scan_str(NASM_str str);
};


