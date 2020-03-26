#pragma once

#define BOOTSTRAP   \
    "@256\n"    \
    "D=A\n"     \
    "@SP\n"     \
    "M=D\n"     \
    "@Return0\n"  \
    "D=A\n"       \
    "@SP\n"       \
    "A=M\n"       \
    "M=D\n"       \
    "@SP\n"       \
    "M=M+1\n"     \
    "@LCL\n"      \
    "D=M\n"       \
    "@SP\n"       \
    "A=M\n"       \
    "M=D\n"       \
    "@SP\n"       \
    "M=M+1\n"     \
    "@ARG\n"      \
    "D=M\n"       \
    "@SP\n"       \
    "A=M\n"       \
    "M=D\n"       \
    "@SP\n"       \
    "M=M+1\n"     \
    "@THIS\n"     \
    "D=M\n"       \
    "@SP\n"       \
    "A=M\n"       \
    "M=D\n"       \
    "@SP\n"       \
    "M=M+1\n"     \
    "@THAT\n"     \
    "D=M\n"       \
    "@SP\n"       \
    "A=M\n"       \
    "M=D\n"       \
    "@SP\n"       \
    "M=M+1\n"     \
    "@SP\n"       \
    "D=M\n"       \
    "@5\n"        \
    "D=D-A\n"     \
    "@ARG\n"      \
    "M=D\n"       \
    "@SP\n"       \
    "D=M\n"       \
    "@LCL\n"      \
    "M=D\n"       \
    "@Sys.init\n" \
    "0;JMP\n"     \
    "(Return0)\n" \

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
    "D=A\n"    \
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
    "AM=M-1\n"     \
    "D=M\n"        \
    "@R15\n"       \
    "A=M\n"        \
    "M=D\n"

#define PUSH_TEMP(i)  \
    "@5\n"         \
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

#define FUNCTION(x) \
    LABEL(inst.label_name)

#define CALL(x, r, n)  \
    OFFSET(r)   \
    "D=A\n"     \
    PUSH_STACK  \
    "@LCL\n"    \
    "D=M\n"     \
    PUSH_STACK  \
    "@ARG\n"    \
    "D=M\n"     \
    PUSH_STACK  \
    "@THIS\n"   \
    "D=M\n"     \
    PUSH_STACK  \
    "@THAT\n"   \
    "D=M\n"     \
    PUSH_STACK  \
    "@SP\n"     \
    "D=M\n"     \
    OFFSET(n)   \
    "D=D-A\n"     \
    "@ARG\n"    \
    "M=D\n"     \
    "@SP\n"     \
    "D=M\n"     \
    "@LCL\n"    \
    "M=D\n"     \
    GOTO(x)     \
    LABEL(r)

#define ENDFRAME_OFFSET(x) \
    "@R13\n"     \
    "D=M\n"     \
    "@" x "\n"  \
    "A=D-A\n"   \
    "D=M\n"

#define RETURN(x)   \
    "@LCL\n"    \
    "D=M\n"     \
    "@R13\n"     \
    "M=D\n"     \
    "@5\n"      \
    "A=D-A\n"   \
    "D=M\n"     \
    "@R15\n"     \
    "M=D\n"     \
    "@SP\n"     \
    "AM=M-1\n"  \
    "D=M\n"     \
    "@ARG\n"    \
    "A=M\n"     \
    "M=D\n"     \
    "@ARG\n"    \
    "D=M+1\n"   \
    "@SP\n"     \
    "M=D\n"     \
    ENDFRAME_OFFSET("1") \
    "@THAT\n"   \
    "M=D\n"     \
    ENDFRAME_OFFSET("2") \
    "@THIS\n"   \
    "M=D\n"     \
    ENDFRAME_OFFSET("3") \
    "@ARG\n"    \
    "M=D\n"     \
    ENDFRAME_OFFSET("4") \
    "@LCL\n"    \
    "M=D\n"     \
    "@R15\n"     \
    "A=M\n"     \
    "0;JMP\n"   \

