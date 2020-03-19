#include "header.h"
#include "Parser.h"

void handle_arguments(int argc, char *argv[],
                std::string &in, std::string &out);

void error(const char *msg);

int main(int argc, char **argv){

    std::string in_filename, out_filename;
    struct instruction inst;
    handle_arguments(argc, argv, in_filename, out_filename);

    std::ifstream in(in_filename, std::ios::in);

    if(!in)
        error("Error opening input file!");
    
    Parser p(in);

    while(p.next()){
        p.command(inst);
    }

    return 0;
}

void handle_arguments(int argc, char **argv, std::string &in, std::string &out)
{
    if (argc < 2 or argc > 3)
    {
        error("Usage: VM VM_FILE.vm (optional) OUTPUT_FILE_NAME\n");
        exit(1);
    }

    in = argv[1];

    if (argc == 3)
        out = argv[2];
    else
        out = in.substr(0, in.find_last_of('.'));
    out += ".asm";
}

void error(const char *msg)
{
    std::cerr << msg << std::endl;
    exit(0);
}