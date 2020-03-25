#pragma once

#define ADD_\
    "@SP\n"    \
    "AM=M-1\n" \
    "D=M\n"    \
    "A=A-1\n"  \
    "M=D+M\n"  \

#define SUB_\
    "@SP\n"    \
    "AM=M-1\n" \
    "D=M\n"    \
    "A=A-1\n"  \
    "M=M-D\n"  \

#define NEG_\
    "@SP\n"    \
    "A=M-1\n" \
    "M=-M\n"  \

#define LT_(x)\
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=M-D\n"       \
    "M=-1\n"        \
    "@LT_" + x + "\n"   \
    "D;JLT\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(LT_" + x + ")\n"

#define GT_(x)\
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=M-D\n"       \
    "M=-1\n"        \
    "@GT_" + x + "\n"   \
    "D;JGT\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(GT_" + x + ")\n"

#define EQ_(x)\
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=M-D\n"       \
    "M=-1\n"        \
    "@EQ_" + x + "\n"   \
    "D;JEQ\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(EQ_" + x + ")\n"

#define AND_\
    "@SP\n"    \
    "AM=M-1\n" \
    "D=M\n"    \
    "A=A-1\n"  \
    "M=D&M\n"  \

#define OR_\
    "@SP\n"    \
    "AM=M-1\n" \
    "D=M\n"    \
    "A=A-1\n"  \
    "M=D|M\n"  \

#define NOT_\
    "@SP\n"   \
    "A=M-1\n" \
    "M=!M\n"  \

#define PUSH_STACK  \
        "@SP\n"     \
        "AM=M+1\n"  \
        "A=A-1\n"   \
        "M=D\n" 

#define PUSH_CONSTANT(x) \
        OFFSET(x)  \
        "D=A\n"         \
        PUSH_STACK

#define PUSH_SEGMENT(x, i)  \
        "@" x "\n"     \
        "D=M\n"        \
        OFFSET(i)      \
        "A=D+A\n"      \
        "D=M\n"        \
        PUSH_STACK

#define POP_SEGMENT(x, i)   \
        "@" x "\n"     \
        "D=M\n"        \
        OFFSET(i)      \
        "D=D+A\n"      \
        "@R15\n"       \
        "M=D\n"        \
        "@SP\n"        \
        "AM=M-1\n"      \
        "D=M\n"        \
        "@R15\n"       \
        "A=M\n"        \
        "M=D\n"

#define PUSH_TEMP(i)  \
        "@5\n"     \
        "D=A\n"        \
        OFFSET(i)      \
        "A=D+A\n"      \
        "D=M\n"        \
        PUSH_STACK

#define POP_TEMP(i)   \
        "@5\n"     \
        "D=A\n"        \
        OFFSET(i)      \
        "D=D+A\n"      \
        "@R15\n"       \
        "M=D\n"        \
        "@SP\n"        \
        "AM=M-1\n"     \
        "D=M\n"        \
        "@R15\n"       \
        "A=M\n"        \
        "M=D\n"

#define PUSH_POINTER(x) \
        "@" x "\n"      \
        "D=M\n"         \
        PUSH_STACK

#define POP_POINTER(x) \
        "@SP\n"        \
        "AM=M-1\n"     \
        "D=M\n"        \
        "@" x "\n"     \
        "M=D\n"

#define PUSH_STATIC(x, i) \
        "@" + x + i + "\n" \
        "D=M\n"            \
        PUSH_STACK

#define POP_STATIC(x, i) \
        "@SP\n"        \
        "AM=M-1\n"     \
        "D=M\n"        \
        "@" + x + i + "\n" \
        "M=D\n"

#define OFFSET(x)   \
        "@" + x + "\n"

#define LABEL(x)    \
        "(" + x + ")\n"

#define GOTO(x)   \
        OFFSET(x) \
        "0;JMP\n"

#define IFGOTO(x)   \
        "@SP\n"     \
        "AM=M-1\n"  \
        "D=M\n"     \
        OFFSET(x)   \
        "D;JNE\n"
