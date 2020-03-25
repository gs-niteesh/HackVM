#include "header.h"
#include "Parser.h"

class CodeGenerator{
    public:
        CodeGenerator(std::string &);
        std::string printInstruction(const struct instruction &);
        void generateCode(struct instruction &);
        ~CodeGenerator();
    private:
        std::ostringstream ostream;
        std::ofstream out;
        std::string filename;
        void push(SEGMENT seg, const std::string &offset);
        void writeArithmetic(struct instruction &);
        void writePushPop(struct instruction &);
        void writeLabel(struct instruction &);
        void writeConditions(struct instruction &);
        void pop(SEGMENT seg, const std::string &offset);
};