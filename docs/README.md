# Low Level C-like Language (LLCL) Compiler


#### Table of Contents  
<div id="user-content-toc">
    <ul>
        <li><a href="#about-llcl">About LLCL</a></li>
        <li><a href="#why">Why?</a></li>
        <li><a href="#assembly-generation">Assembly Generation</a></li>
        <ul>
            <li><a href="#for-loops">For Loops</a></li>
        </ul>
        <li><a href="#the-preprocessor">The Preprocessor</a></li>
        <ul>
            <li><a href="#macros">Macros</a></li>
        </ul>
        <li><a href="#the-lexer">The Lexer</a></li>
        <ul>
            <li><a href="#tokenization">Tokenization</a></li>
        </ul>
        <li><a href="#the-parser">The Parser</a></li>
        <ul>
            <li><a href="#parsing-expressions">Parsing Expressions</a></li>
            <li><a href="#evaluating-expressions">Evaluating Expressions</a></li>
        </ul>
    </ul>
</div>


## About LLCL
LLCL is a very low level (lower than C) programming language:

- No built in memory allocation. At runtime a 100MB block of memory called MEMBLOCK is allocated. All dynamic memory allocation happens within MEMBLOCK and is written by the developer.

- No pointer type. All pointers are simply 64-bit integers (`i64`) used to index into MEMBLOCK.
The naming convention for pointers is to prefix them with "p_" for "pointer". Double pointers are prefixed with "pp_", triple pointers with "ppp_" and so on.

- No structs/classes.

- The built in datatypes are:
    - `u00` - unsigned 0-bit integer (void)
    - `u08`, `u16`, `u32`, `u64` - unsigned 8, 16, 32 and 64-bit integers.
    - `i08`, `i16`, `i32`, `i64` - signed 8, 16, 32 and 64-bit integers.
    - `f32`, `f64` - 32 and 64-bit floating point.


## Why?
This is a learning project, not intended for professional use.
Here I document the development of the LLCL Compiler.



## Assembly Generation

- `rax` and `r8` are used for evaluating expressions
    ```nasm
    pop   r8
    pop   rax
    idiv  r8
    push  rax
    ...
    ```


### For Loops
- LLCL Syntax
    ```
    for a..b
        // code will execute b-a times
    endfor
    ```


- `rsi` is used to store loop counters
    ```nasm
    mov   rsi,   0
    ;
    ; loop stuff
    ;
    add   rsi,   1
    ```
- For nested loops, the loop counter stored in `rsi` is pushed to the stack and then popped after the inner loop completes
    ```nasm
    mov   rsi,   0
    push  rsi  ; save the loop counter
    ;---------------------; inner loop
        mov   rsi,   0
        ;
        ; inner loop stuff
        ;
        add   rsi,   1
    ;--------------------------------;
    pop   rsi  ; restore the loop counter
    add   rsi,   1
    ```

## The Preprocessor
In this section I document the process of developing the LLCL pre-processor.


### Macros
Two types of macro exist, simple replacement macros and function-like macros.
Function-like macros can take only a single parameter.


## The Lexer
In this section I document the process of developing the LLCL Lexer.


### Tokenization
Tokenization text goes here.


## The Parser
In this section I document the process of developing the LLCL Parser.



### Parsing Expressions
Parsing text goes here.


### Evaluating Expressions
- Recursively parse expressions, generating a binary tree.
- Generate x86_64 NASM by performing a post-order traversal of the tree.

<p align="center">
    <img src="parsetree.png" width="500px"></img>
    <p align="center">Using a parse tree to generate assembly from "a + ((b * (2+3)) / (25*50))"</p>
    <p align="center">The final result is stored at the top of the stack</p>
</p>

