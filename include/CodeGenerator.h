#include "header.h"
#include "Parser.h"

class CodeGenerator{
    public:
        CodeGenerator(std::string &);
        void currentFile(const std::string &);
        void generateCode(struct instruction &);
        ~CodeGenerator();
    private:
        std::ostringstream ostream;
        std::ofstream out;
        std::string filename;
        void push(SEGMENT, const std::string &);
        void writeArithmetic(struct instruction &);
        void writePushPop(struct instruction &);
        void writeLabel(struct instruction &);
        void writeConditions(struct instruction &);
        void writeFunction(struct instruction &);
        void writeCall(struct instruction &);
        void writeReturn(struct instruction &);
        void bootstrapCode();
        void pop(SEGMENT, const std::string &);
        std::string printInstruction(const struct instruction &);
};