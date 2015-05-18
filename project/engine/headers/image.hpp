#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

/*Code extracted and modified from: Zoe Wood

NOTE: Files must be 24-bit Bitmap format (BMP)*/

#include "pointer_macros.h"
DEF_PTR(Image, ImagePtr);

class Image{
public:
    Image();
    Image(std::string _path);
    Image(unsigned char * _pixels, unsigned int w, unsigned int h,
           std::string extension);

    void load();

    std::string getFileName();
    unsigned char * getImage();
    unsigned int getWidth();
    unsigned int getHeight();

    bool isPNG();
    bool isBMP();

    void write(std::string filepath);

private:
    static const std::string BMP_EXT;
    static const std::string PNG_EXT;
    static const std::string INVALID_EXT;

    static const std::string INVALID_PATH;
    static const std::string GENERATED_MANUALLY;

    std::string path;
    unsigned char * pixels;
    unsigned int width, height;

    bool loaded;

    unsigned int loadBMP();
    unsigned int loadPNG();
    unsigned int writeBMP(std::string filename);
    unsigned int writePNG(std::string filename);
    std::string getFileExtension(std::string path);
};

#endif
