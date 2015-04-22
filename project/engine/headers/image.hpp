#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include "GLIncludes.h"

/*Code extracted and modified from: Zoe Wood

NOTE: Files must be 24-bit Bitmap format (BMP)*/

class Image{
public:
    Image();
    Image(std::string _path);

    void load();

    std::string getFileName();
    char * getImage();
    GLuint getWidth();
    GLuint getHeight();

private:
    static const std::string BMP_EXT;
    static const std::string PNG_EXT;
    static const std::string INVALID_EXT;

    static const std::string INVALID_PATH;

    std::string path;
    char * pixels;
    GLuint width, height;

    bool loaded;

    int loadBMP();
    std::string getFileExtension();
};

#endif
