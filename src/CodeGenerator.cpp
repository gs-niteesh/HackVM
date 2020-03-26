#include "CodeGenerator.h"
#include "ASM.h"

static const char *OPS[static_cast<uint32_t>(OP::MAX_OP)] = {
    "push ",
    "pop ",
    "add ",
    "sub ",
    "neg ",
    "eq ",
    "gt ",
    "lt ",
    "and ",
    "or ",
    "not ",
    "label ",
    "goto ",
    "ifgoto ",
    "function ",
    "call ",
    "return "
};

static const char *
    SEGMENTS[static_cast<uint32_t>(SEGMENT::MAX_SEGMENT)] = {
        "constant ",
        "local ",
        "temp ",
        "static ",
        "pointer ",
        "this ",
        "that ",
        "argument ",
        ""
};

static std::string currentFunction = "";

CodeGenerator::CodeGenerator(std::string &file)
{
    out.open(file, std::ios::app);

    if (!out.is_open())
    {
        std::cerr << "Cannot open output file\n";
        exit(EXIT_FAILURE);
    }
    bootstrapCode();
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
    writeFunction(inst);
    writeCall(inst);
    writeReturn(inst);
}

void CodeGenerator::currentFile(const std::string &name)
{
    std::size_t end = name.rfind('/');

    filename = name.substr(end + 1);
    filename = filename.substr(0, filename.length() - 2);
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

void CodeGenerator::writeFunction(struct instruction &inst)
{
    if (inst.op == OP::FUNCTION){
        int nargs = atoi(inst.offset.c_str());
        currentFunction = inst.label_name;
        ostream << FUNCTION(inst.label_name);
        while(nargs--){
            ostream << "@0\n" << "D=A\n" << PUSH_STACK;
        }
    }
}

void CodeGenerator::writeCall(struct instruction &inst)
{
    if (inst.op == OP::CALL){
        int x = atoi(inst.offset.c_str()) + 5;
        static int n = 0;
        std::string returnFunc = currentFunction + "$ret" + std::to_string(n);
        ostream << CALL(inst.label_name, returnFunc, std::to_string(x));
        n++;
    }
}

void CodeGenerator::writeReturn(struct instruction &inst)
{
    if (inst.op == OP::RETURN){
        std::string returnFunc = currentFunction + "$ret";
        ostream << RETURN(returnFunc);
    }
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

void CodeGenerator::bootstrapCode()
{
    ostream << BOOTSTRAP;
}

std::string CodeGenerator::printInstruction(const struct instruction &inst)
{

    std::string result = "";
    if (inst.op != OP::INVALID)
    {
        result = "// ";
        result += OPS[static_cast<uint32_t>(inst.op)];
        result += SEGMENTS[static_cast<uint32_t>(inst.segment)];
        if (inst.label_name != "") {
            result += inst.label_name;
            result += " ";
        }
        result += inst.offset;
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
