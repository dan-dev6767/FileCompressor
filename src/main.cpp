#include "compress.hpp"
#include <CLI/CLI.hpp>


int main (int argc, char* argv[])
{
    std::vector<std::string> files;
    int threads = 0;
    int level = 0;
    bool isDecompress=false;

    CLI::App app("Simple Compression util");

    app.add_option("-f,--files",files, "Files names")->delimiter(';')->required();

    app.add_flag("-d,--decompress", isDecompress);

    app.add_option("-t,--threads", threads, "Threads count");

    app.add_option("-l,--level", level, "Compression Level");

    CLI11_PARSE(app, argc, argv);

    if (!isDecompress)
    {
        for (const auto& file : files)
        {
            FileCompression::lazyCompression(file, level, threads);   
        }
    } else {
        for (const auto& file : files)
        {
            FileCompression::lazyDecompression(file);   
        }
    }

    return 0;
}