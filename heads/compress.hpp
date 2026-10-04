#pragma once

#ifdef _WIN32
    #ifdef COMPRESSION_EXPORT
        #define Compression_API __declspec(dllexport)
    #else
        #define Compression_API __declspec(dllimport)
    #endif
#else
    #define Compression_API
#endif

#include <filesystem>
namespace FileCompression 
{
    Compression_API void lazyCompression(std::filesystem::path path, int level, int threads);
    Compression_API void lazyDecompression(std::filesystem::path path);
};