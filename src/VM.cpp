#include <header.h>
#include <Parser.h>
#include <CodeGenerator.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_FILES 10
#define VM_EXTENSION ".vm"

void handle_arguments(int argc, char *argv[],
                std::string &in);

void error(const std::string &msg);

bool isDirectory(const std::string &name);

void getFiles(
    const char *dirname,
    std::vector<std::string> &files
);

int main(int argc, char **argv){

    std::string in_filename, out_filename;
    const char *dirname;
    struct instruction inst;
    std::vector<std::string> files;
    files.reserve(MAX_FILES);

    handle_arguments(argc, argv, in_filename);

    bool isdir = isDirectory(in_filename);

    dirname = realpath(in_filename.c_str(), NULL);

    if(isdir) {
        getFiles(dirname, files);
        out_filename = dirname;
        out_filename += "/";
        out_filename += basename(dirname);
        out_filename += ".asm";
    }else{
        files.push_back(dirname);
        out_filename = dirname;
        std::size_t pos = out_filename.find_last_of('.');
        out_filename.replace(pos, 3, ".asm");
    }

    CodeGenerator writer(out_filename);
    
    for(auto &filename : files){
        std::ifstream in(filename, std::ios::in);

        if(!in)
            error("Error opening input file " + filename);
    
        Parser p(in);
        writer.currentFile(filename);

        while(p.next()){
            p.command(inst);
            writer.generateCode(inst);
        }
    }

    return 0;
}

void handle_arguments(int argc, char **argv, std::string &in)
{
    if (argc != 2)
    {
        error("Usage: VM VM_FILE.vm or Directory");
        exit(1);
    }

    in = argv[1];
}

bool isDirectory(const std::string &name)
{
    struct stat info;
    if (stat(name.c_str(), &info) != 0) {
        error("Error reading directory");
    }
    mode_t mode = info.st_mode;

    if (mode & S_IFDIR) {
        return true;
    }else if (mode & S_IFREG) {
        return false;
    }else {
        error("Provide a directory or file\n");
    }
}

void getFiles(
    const char *dirname,
    std::vector<std::string> &files
)
{
    DIR *d;
    struct dirent *dir;
    std::string mdirname{dirname};
    const char *dot, *slash;
    if ((d = opendir(dirname))) {
        while ((dir = readdir(d)) != NULL) {
            dot = strrchr(dir->d_name, '.');
            if (dir->d_type == DT_REG && dot &&  !strcmp(dot, VM_EXTENSION)) {
                files.push_back(mdirname + "/" + dir->d_name);
            }
        }
        closedir(d);
    }
}

void error(const std::string &msg)
{
    std::cerr << msg << std::endl;
    exit(0);
}