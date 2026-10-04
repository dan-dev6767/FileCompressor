#include "compress.hpp"
#include <CLI/CLI.hpp>


int main (int argc, char* argv[])
{
    std::vector<std::string> files;
    int threads = 0;
    int level = 0;

    CLI::App app("Simple Compression util");

    app.add_option_function<std::string>("-f,--files", 
    [&threads, &level](const std::string& file){
        FileCompression::lazyCompress(file, level, threads);
    }, "Files names")->delimiter(';')->required();

    app.add_option("-t,--threads", threads, "Threads count");

    app.add_option("-l,--level", level, "Compression Level");

    CLI11_PARSE(app, argc, argv);
    return 0;
}