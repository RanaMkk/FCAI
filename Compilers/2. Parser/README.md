# TINY Language Analyzer Project Description

## Project Overview

This project involves the implementation of an analyzer for the TINY programming language based on the grammar rules of the language. The analyzer, implemented in C++, will take the source code written in TINY as input and output the terminal (leaf) nodes of the parse tree to the terminal. The project utilizes the grammar rules provided to build a parse tree and visualize its structure.

## Project Components

### 1. Analyzer Implementation

The primary goal of this project is to develop a C++ program that serves as the analyzer for the TINY language. The analyzer will employ the grammar rules to construct a parse tree for the input source code. It will then output the terminal nodes of the parse tree to the terminal.

### 2. Input and Output

#### Input
The analyzer will take the path of a TINY programming language source code file as input. An example input file is provided in `input.txt`.

#### Output
The output of the program will be the terminal nodes of the parse tree, printed to the terminal. The output format follows the structure provided in the `CompilersTask_2_Parser.cpp` file. Each level of the tree is indented by 3 spaces, indicating the depth in the parse tree.

### 3. Grammar Rules

The grammar rules for the TINY programming language are specified below:

```
program -> stmtseq
stmtseq -> stmt { ; stmt }
stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
ifstmt -> if expr then stmtseq [ else stmtseq ] end
repeatstmt -> repeat stmtseq until expr
assignstmt -> identifier := expr
readstmt -> read identifier
writestmt -> write expr
expr -> mathexpr [ (<|=) mathexpr ]
mathexpr -> term { (+|-) term }    (left associative)
term -> factor { (*|/) factor }    (left associative)
factor -> newexpr { ^ newexpr }    (right associative)
newexpr -> ( mathexpr ) | number | identifier
```

### 4. Example Output

An example output for the given grammar rules and input source code is provided in the project description. The output demonstrates the expected format for representing the terminal nodes of the parse tree.

## Conclusion

This project aims to create a functional analyzer for the TINY programming language, enabling the visualization of the parse tree structure based on the specified grammar rules. The correct interpretation and representation of the parse tree are crucial for understanding the syntactic structure of TINY language programs.
