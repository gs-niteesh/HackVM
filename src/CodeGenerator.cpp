#include "CodeGenerator.h"
#include "ASM.h"

static const char *OPS[static_cast<uint32_t>(OP::MAX_OP)] = {
    "PUSH",
    "POP",
    "ADD",
    "SUB",
    "NEG",
    "EQ",
    "GT",
    "LT",
    "AND",
    "OR",
    "NOT",
    "LABEL",
    "GOTO",
    "IFGOTO"
};

static const char *
    SEGMENTS[static_cast<uint32_t>(SEGMENT::MAX_SEGMENT)] = {
        " CONSTANT ",
        " LOCAL ",
        " TEMP ",
        " STATIC ",
        " POINTER ",
        " THIS ",
        " THAT ",
        " ARGUMENT ",
};

CodeGenerator::CodeGenerator(std::string &file)
{
    out.open(file, std::ios::out | std::ios::trunc);

    std::size_t end = file.rfind('/');

    filename = file.substr(end + 1);
    filename = filename.substr(0, filename.length() - 3);

    if (!out.is_open())
    {
        std::cerr << "Cannot open output file\n";
        exit(EXIT_FAILURE);
    }
}

void CodeGenerator::generateCode(struct instruction &inst)
{
    if (inst.op == OP::INVALID) {
        return ;
    }
    ostream << "\n" << printInstruction(inst);
    writePushPop(inst);
    writeArithmetic(inst);
    writeLabel(inst);
    writeConditions(inst);
}

void CodeGenerator::writePushPop(struct instruction &inst)
{
    switch (inst.op)
    {
    case OP::PUSH:
        push(inst.segment, inst.offset);
        break;
    case OP::POP:
        pop(inst.segment, inst.offset);
        break;
    }
}

void CodeGenerator::writeLabel(struct instruction &inst)
{
    if (inst.op == OP::LABEL) {
        ostream << LABEL(inst.offset);
    }
}

void CodeGenerator::writeConditions(struct instruction &inst)
{
    if (inst.op == OP::GOTO) {
        ostream << GOTO(inst.offset);
    }else if (inst.op == OP::IFGOTO) {
        ostream << IFGOTO(inst.offset);
    }
}

void CodeGenerator::writeArithmetic(struct instruction &inst)
{
    static uint32_t i = 0;
    std::string s = std::to_string(i);
    switch (inst.op)
    {
    case OP::ADD:
        ostream << ADD_;
        break;
    case OP::SUB:
        ostream << SUB_;
        break;
    case OP::NEG:
        ostream << NEG_;
        break;
    case OP::EQ:
        ostream << EQ_(s);
        break;
    case OP::LT:
        ostream << LT_(s);
        break;
    case OP::GT:
        ostream << GT_(s);
        break;
    case OP::AND:
        ostream << AND_;
        break;
    case OP::OR:
        ostream << OR_;
        break;
    case OP::NOT:
        ostream << NOT_;
        break;
    }
    i++;
}

void CodeGenerator::push(SEGMENT seg, const std::string &offset)
{
    switch (seg)
    {
    case SEGMENT::CONSTANT:
        ostream << PUSH_CONSTANT(offset);
        break;
    case SEGMENT::LOCAL:
        ostream << PUSH_SEGMENT("LCL", offset);
        break;
    case SEGMENT::ARGUMENT:
        ostream << PUSH_SEGMENT("ARG", offset);
        break;
    case SEGMENT::THIS:
        ostream << PUSH_SEGMENT("THIS", offset);
        break;
    case SEGMENT::THAT:
        ostream << PUSH_SEGMENT("THAT", offset);
        break;
    case SEGMENT::STATIC:
        ostream << PUSH_STATIC(filename, offset);
        break;
    case SEGMENT::POINTER:
        if (offset == "0") ostream << PUSH_POINTER("THIS");
        else             ostream << PUSH_POINTER("THAT");
        break;
    case SEGMENT::TEMP:
        ostream << PUSH_TEMP(offset); 
        break;
    }
}

void CodeGenerator::pop(SEGMENT seg, const std::string &offset)
{
    switch (seg)
    {
    case SEGMENT::LOCAL:
        ostream << POP_SEGMENT("LCL", offset); ;
        break;
    case SEGMENT::ARGUMENT:
        ostream << POP_SEGMENT("ARG", offset);;
        break;
    case SEGMENT::THIS:
        ostream << POP_SEGMENT("THIS", offset); 
        break;
    case SEGMENT::THAT:
        ostream << POP_SEGMENT("THAT", offset); 
        break;
    case SEGMENT::STATIC:
        ostream << POP_STATIC(filename, offset);
        break;
    case SEGMENT::POINTER:
        if (offset == "0") ostream << POP_POINTER("THIS");
        else             ostream << POP_POINTER("THAT");
        break;
    case SEGMENT::TEMP:
        ostream << POP_TEMP(offset); 
        break;
    }
}

std::string CodeGenerator::printInstruction(const struct instruction &inst)
{

    std::string result = "";
    if (inst.op != OP::INVALID)
    {
        result = "// ";
        result += OPS[static_cast<uint32_t>(inst.op)];

        if (inst.segment != SEGMENT::NIL)
        {
            result += SEGMENTS[static_cast<uint32_t>(inst.segment)];
            result += inst.offset;
        }
        result += "\n";
    }
    return result;
}

CodeGenerator::~CodeGenerator()
{
    if (out.is_open())
    {
        out << ostream.str();
        out.close();
    }
}
