#include "compress.hpp"

#include <zstd.h>
#include <fstream>
#include <vector>

namespace FileCompression
{
    Compression_API void lazyCompress(std::filesystem::path path, int level, int threads)
    {
        // Open Files
        std::ifstream source(path, std::ios::binary);
        std::ofstream destination(path.string()+".zst", std::ios::binary);
        if (!source.is_open() || !destination.is_open()) throw std::runtime_error("Failed to open source or destination file!");

        // Init ZSTD
        std::unique_ptr<ZSTD_CCtx, decltype(&ZSTD_freeCCtx)> cctx(ZSTD_createCCtx(), ZSTD_freeCCtx);
        if (!cctx) throw std::runtime_error("Cannot Initializate zstd context!");

        // setting compression
        size_t resLevel = ZSTD_CCtx_setParameter(cctx.get(), ZSTD_c_compressionLevel, level);
        size_t relThreads = ZSTD_CCtx_setParameter(cctx.get(), ZSTD_c_nbWorkers, threads);

        // init buffers
        std::vector<char> inBuff(ZSTD_CStreamInSize());
        ZSTD_inBuffer input = {inBuff.data(), 0, 0};

        std::vector<char> outBuff(ZSTD_CStreamOutSize());
        ZSTD_outBuffer output = {outBuff.data(),ZSTD_CStreamOutSize(),0};

        // Compression file
        while (source.read(inBuff.data(), inBuff.size()) || source.gcount() > 0)
        {
            input.size = source.gcount();
            input.pos = 0;

            while (input.pos < input.size)
            {
                // Compress
                output.pos = 0;
                size_t const code = ZSTD_compressStream2(cctx.get(), &output, &input, ZSTD_e_continue);
                if (ZSTD_isError(code)) throw std::runtime_error(ZSTD_getErrorName(code));

                // Saving
                destination.write(outBuff.data(), output.pos);
            }
        }

        input.size = 0;
        input.pos = 0;

        // Writing end
        size_t remaining = 1;
        while (remaining > 0)
        {
            output.pos = 0;
            remaining = ZSTD_compressStream2(cctx.get(), &output, &input, ZSTD_e_end);
            if (ZSTD_isError(remaining)) throw std::runtime_error(ZSTD_getErrorName(remaining));

            destination.write(outBuff.data(), output.pos);
        }
    }
}