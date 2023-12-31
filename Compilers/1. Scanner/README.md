# TINY Language Scanner Project Description

## Project Overview

This project involves the implementation of a scanner (tokenization) for the TINY programming language. The TINY language is a simple language with basic constructs for arithmetic, boolean expressions, input/output operations, and control flow statements. The scanner is a crucial component in the compilation process, responsible for breaking down the source code into individual tokens.

## Project Components

### 1. Scanner Implementation

The main focus of this project is to develop a C++ program that serves as the scanner for the TINY language. The scanner will read a TINY source code file (e.g., `input.txt`) and generate an output file containing tokens in the specified format. Each token will be represented by the line number it was found in, the actual string from the input file, and the corresponding token type.

### 2. Input and Output

#### Input
The scanner will take a TINY programming language source code file as input. An example input file is provided in `input.txt`.

#### Output
The output of the scanner will be a text file containing tokens. Each token will be represented on a single line, following the format:
```
[number of line the token was found in starting from 1 in square brackets] [the actual string that represents the token in the input file] [a string that represents the token type in round brackets]
```

### 3. TINY Language Specification

To guide the implementation of the scanner, a separate document called `TINY language spec.md` has been provided. This document includes all the rules and specifications for the TINY programming language, covering statements, expressions, control flow, and variable declarations.

## TINY Language Specification

For detailed information on the rules and specifications of the TINY programming language, please refer to the `TINY language spec.md` document. This document outlines the syntax and semantics of the language, including rules for variables, statements, expressions, and control flow.

## Example Output

An example output file for the scanner is provided in the project description. This output file demonstrates the expected format for token representation, including line numbers, actual strings, and token types.

## Conclusion

This project aims to create a functional scanner for the TINY programming language, enabling subsequent phases of the compilation process. The clarity and correctness of the tokenization process are crucial for the successful compilation of TINY language programs.
