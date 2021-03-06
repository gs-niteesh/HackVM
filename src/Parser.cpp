#include "Parser.h"

Parser::Parser(std::ifstream &in) : in(in)
{
}

bool Parser::next()
{
    return !in.eof();
}

void Parser::command(struct instruction &inst)
{
    std::string cmd = readline();

    if (in.eof()) {
        inst.op = OP::INVALID;
        return ;
    }
   
    parse(cmd, inst);
}

static OP operation(const std::string &str)
{
    if (str == "push") {
        return OP::PUSH;
    } else if (str == "pop") {
        return OP::POP;
    } else if (str == "add") {
        return OP::ADD;
    } else if (str == "sub") {
        return OP::SUB;
    } else if (str == "neg") {
        return OP::NEG;
    } else if (str == "eq") {
        return OP::EQ;
    } else if (str == "gt") {
        return OP::GT;
    } else if (str == "lt") {
        return OP::LT;
    } else if (str == "and") {
        return OP::AND;
    } else if (str == "or") {
        return OP::OR;
    } else if (str == "not") {
        return OP::NOT;
    } else if (str == "label") {
        return OP::LABEL;
    } else if (str == "if-goto") {
        return OP::IFGOTO;
    } else if (str == "goto") {
        return OP::GOTO;
    } else if (str == "function") {
        return OP::FUNCTION;
    } else if (str == "call") {
        return OP::CALL;
    } else if (str == "return") {
        return OP::RETURN;
    }
}

static SEGMENT segment(const std::string &str)
{
    if (str == "constant") {
        return SEGMENT::CONSTANT;
    } else if (str == "local") {
        return SEGMENT::LOCAL;
    } else if (str == "this") {
        return SEGMENT::THIS;
    } else if (str == "that") {
        return SEGMENT::THAT;
    } else if (str == "temp") {
        return SEGMENT::TEMP;
    } else if (str == "static") {
        return SEGMENT::STATIC;
    } else if (str == "pointer") {
        return SEGMENT::POINTER;
    } else if (str == "argument") {
        return SEGMENT::ARGUMENT;
    }
    return SEGMENT::NIL;
}

void Parser::parse(const std::string &line, instruction &inst)
{
    std::string token;
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    while(std::getline(iss, token, ' ')){
        tokens.push_back(token);
    }
    std::size_t size = tokens.size();

    if(size != 0){
        inst.op = operation(tokens[0]);
        inst.segment = SEGMENT::NIL;
        inst.offset = "";
        inst.label_name = "";

        if (size == 3){
            inst.segment = segment(tokens[1]);
            if (inst.segment == SEGMENT::NIL && (inst.op == OP::FUNCTION
                                             ||  inst.op == OP::CALL)) {
                inst.label_name = tokens[1];
            }
            inst.offset = tokens[2];
        }else if(size == 2){
            inst.segment = SEGMENT::NIL;
            inst.offset = tokens[1];
        }
    }else{
        inst.op = OP::INVALID;
    }
}

std::string Parser::readline()
{
    std::string result;
    std::getline(in, result);
    sanitize(result);

    while (result == "" && !in.eof())
    {
        result = readline();
    }
    return result;
}

void Parser::sanitize(std::string &line)
{
    std::size_t size = line.size();
    std::size_t pos = line.find_first_of("//");
    if (pos != std::string::npos)
    {
        line.erase(pos, size);
    }
    pos = line.find_first_of('\r');
    if (pos != std::string::npos)
    {
        line.erase(pos, size);
    }
    pos = line.find_first_not_of(" \r\t\n");
    if (pos != std::string::npos)
    {
        line.erase(0, pos);
    }
    pos = line.find_last_not_of(" \r\t\n");
    if (pos != std::string::npos)
    {
        line.erase(pos + 1, size);
    }
}
