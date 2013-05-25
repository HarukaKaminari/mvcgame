

#include <mvcgame/texture/PngTextureLoader.hpp>
#include <mvcgame/texture/Texture.hpp>
#include <png.h>

/**
 * this png loader is based on this blog post
 * http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
 */

#define PNGSIGSIZE 8

namespace mvcgame {

    bool PngTextureLoader::validate(std::istream& input)
    {
        png_byte pngSig[PNGSIGSIZE];
        memset(pngSig, 0, PNGSIGSIZE);
        bool isPng = false;
        input.read(reinterpret_cast<std::istream::char_type*>(pngSig), PNGSIGSIZE);
        if (input.good())
        {
            isPng = png_sig_cmp(pngSig, 0, PNGSIGSIZE) == 0;
        }
        
#ifdef MVCGAME_DEBUG_TEXTURE_PNG
        std::cout << "----" << std::endl;        
        std::cout << "PngTextureLoader validate " << (isPng?"yes":"no") << std::endl;
        std::cout << "signature " << std::hex;
        for(unsigned i=0; i<PNGSIGSIZE; i++)
        {
            std::cout << (int) pngSig[i];    
        }
        std::cout << std::dec << std::endl << "----" << std::endl;
#endif
        return isPng;
    }

    void PngTextureLoaderReadData(png_structp pngPtr, png_bytep data, png_size_t length)
    {
        png_voidp a = png_get_io_ptr(pngPtr);
        static_cast<std::istream*>(a)
            ->read(reinterpret_cast<std::istream::char_type*>(data), length);
    }

    std::unique_ptr<Texture> PngTextureLoader::load(std::istream& input)
    {
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!pngPtr)
        {
            return std::unique_ptr<Texture>();
        }
        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (!infoPtr)
        {
            png_destroy_read_struct(&pngPtr, nullptr, nullptr);
            return std::unique_ptr<Texture>();
        }
        png_bytep* rowPtrs = nullptr;
        png_byte* data = nullptr;

        if (setjmp(png_jmpbuf(pngPtr)))
        {
            png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
            if (rowPtrs != nullptr)
            {
                delete[] rowPtrs;
            }
            if (data != nullptr)
            {
                delete[] data;
            }
            return std::unique_ptr<Texture>();
        }
        png_set_read_fn(pngPtr, (png_voidp)&input, PngTextureLoaderReadData);

        png_read_info(pngPtr, infoPtr);
        png_uint_32 imgWidth   = png_get_image_width(pngPtr, infoPtr);
        png_uint_32 imgHeight  = png_get_image_height(pngPtr, infoPtr);
        png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
        png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
        png_uint_32 colorType = png_get_color_type(pngPtr, infoPtr);
        
        switch (colorType)
        {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            channels = 3;           
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            bitdepth = 8;
            break;
        }
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(pngPtr);
            channels += 1;
        }

        rowPtrs = new png_bytep[imgHeight];
        const unsigned int stride = imgWidth * bitdepth * channels / 8;        
        const size_t size = stride * imgHeight;
        data = new uint8_t[size];

        for (size_t i = 0; i < imgHeight; i++)
        {
            png_uint_32 q = (imgHeight- i - 1) * stride;
            rowPtrs[i] = (png_bytep)data + q;
        }
        png_read_image(pngPtr, rowPtrs);

#ifdef MVCGAME_DEBUG_TEXTURE_PNG

        std::string colorTypeStr;
        switch (colorType)
        {
        case PNG_COLOR_TYPE_GRAY:
            colorTypeStr = "gray";
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            colorTypeStr = "gray with alpha";
            break;        
        case PNG_COLOR_TYPE_PALETTE:
            colorTypeStr = "palette";
            break;        
        case PNG_COLOR_TYPE_RGB:
            colorTypeStr = "rgb";
            break;        
        case PNG_COLOR_TYPE_RGB_ALPHA:
            colorTypeStr = "rgb with alpha";
            break;        
        }

        std::cout << "----" << std::endl;        
        std::cout << "PngTextureLoader loaded " << size << " bytes " << colorTypeStr << std::endl;
        std::cout << "size " << imgWidth << "x" << imgHeight << ", depth " << bitdepth;
        std::cout << ", channels " << channels << std::endl;
        std::cout << "----" << std::endl;
#endif

        delete[] rowPtrs;
        png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);        

        return std::unique_ptr<Texture>(new Texture(std::unique_ptr<uint8_t []>(data), size));
    }

}