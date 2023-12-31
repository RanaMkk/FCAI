# TINY Language Specification

## 1. Statements

A TINY program consists of a sequence of statements separated by semicolons (`;`). The statements form the basic building blocks of the program.

## 2. Comments

Comments in TINY are denoted by curly braces `{}`. Anything enclosed within curly braces is treated as a comment and is ignored during compilation.

## 3. Input/Output Operations

TINY supports basic input and output operations:

- `read`: Used for input.
- `write`: Used for output.

## 4. Mathematical Expressions

Mathematical expressions in TINY involve integers only and support the following operators:

- `+`: Addition
- `-`: Subtraction
- `*`: Multiplication
- `/`: Division
- `^`: Exponentiation

## 5. Boolean Expressions

Boolean expressions are used in `if` and `repeat` conditions. They involve comparisons using the following operators:

- `<`: Less than
- `=`: Equal to

Boolean expressions can also include two mathematical expressions.

## 6. Repeat Statement

The repeat statement in TINY follows the syntax:
```
repeat until (boolean)
```

It repeats a block of statements until the specified boolean condition is true.

## 7. If Statement

The if statement in TINY follows the syntax:
```
if (boolean) then [else] end
```

It allows conditional execution of statements based on a boolean condition.

## 8. Variable Names and Declarations

Variable names in TINY can include only alphabetic characters (a:z or A:Z) and underscores. Variables are declared by assigning values to them using `:=`. All variables are of integer type.

## 9. No Procedures and Declarations

TINY does not support procedures, and there are no explicit declarations for variables. Variables are implicitly declared when assigned a value.

---

This document outlines the rules and specifications of the TINY programming language. It serves as a reference for implementing the scanner and ensuring the correct interpretation of TINY source code during the compilation process.
