#pragma once
#include "header.h"

typedef enum class OP {
    PUSH = 0,
    POP,
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT,
    LABEL,
    GOTO,
    IFGOTO,
    INVALID,
    MAX_OP
}OP;

typedef enum class SEGMENT{
    CONSTANT = 0,
    LOCAL,
    TEMP,
    STATIC,
    POINTER,
    THIS,
    THAT,
    ARGUMENT,
    NIL,
    MAX_SEGMENT
}SEGMENT;

struct instruction
{
    OP op;
    SEGMENT segment;
    std::string offset;
}; 

class Parser
{
public:
    Parser(std::ifstream &in);
    bool next();
    void command(struct instruction &inst);

private:
    std::string readline();
    void sanitize(std::string &line);
    void parse(const std::string &line, instruction &inst);
    std::ifstream &in;
};