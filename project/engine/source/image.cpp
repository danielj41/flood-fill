#include "image.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"
#include "lodepng.h"


const std::string Image::BMP_EXT = "bmp";
const std::string Image::PNG_EXT = "png";
const std::string Image::INVALID_EXT = "INVALID_EXT";
const std::string Image::INVALID_PATH = "INVALID_PATH";
const std::string Image::GENERATED_MANUALLY = "GENERATED_MANUALLY";


Image::Image() : path(INVALID_PATH), loaded(false) {}

Image::Image(std::string _path): path(_path), loaded(false){}

Image::Image(unsigned char * _pixels, unsigned int w, unsigned int h,
    std::string extension)
 : pixels(_pixels), width(w), height(h), loaded(true){
    std::string generateName = GENERATED_MANUALLY;
    generateName += ".";
    generateName += extension;

    path = generateName;

    ASSERT(isBMP() || isPNG(), "Invalid extension " << extension << "!");
}

void Image::load(){
    ASSERT(path != INVALID_PATH, "You didn't provide a path for the Image");
    ASSERT(path != GENERATED_MANUALLY, "This image was generated manually. "
                                        << "You can't load it");

    INFO("Loading image " << path << "...");

    std::string ext = getFileExtension(path);

    if(ext == BMP_EXT){
        unsigned int rc = loadBMP();
        if(!rc) loaded = true;
    }
    else if (ext == PNG_EXT){
        unsigned int rc = loadPNG();
        if(!rc) loaded = true;
    }

    ASSERT(loaded, "Failed to load image " << path << "!");
    INFO("Image " << path << " loaded!");
    INFO("\tWidth: " << width);
    INFO("\tHeight: " << height);
}

std::string Image::getFileName(){
    ASSERT(path != INVALID_PATH, "You didn't provide a path for the Image");
    return path;
}

unsigned char * Image::getImage(){
    ASSERT(loaded, "You didn't load the image!");
    return pixels;
}

unsigned int Image::getWidth(){
    ASSERT(loaded, "You didn't load the image!");
    return width;
}

unsigned int Image::getHeight(){
    ASSERT(loaded, "You didn't load the image!");
    return height;
}


bool Image::isPNG(){
    ASSERT(loaded, "You didn't load the image!");
    return getFileExtension(path) == PNG_EXT;
}

bool Image::isBMP(){
    ASSERT(loaded, "You didn't load the image!");
    return getFileExtension(path) == BMP_EXT;
}


void Image::write(std::string filepath){
    ASSERT(loaded, "You didn't load the image!");
    ASSERT(path != INVALID_PATH, "You didn't provide a path for the Image");

    INFO("Writing image " << filepath << "...");

    std::string ext = getFileExtension(filepath);

    bool success = false;

    if(ext == BMP_EXT){
        unsigned int rc = writeBMP(filepath);
        if(!rc) success = true;
    }
    else if (ext == PNG_EXT){
        unsigned int rc = writePNG(filepath);
        if(!rc) success = true;
    }

    ASSERT(success, "Failed to write image " << filepath << "!");
    INFO("Image wrote " << filepath << "!");
}

/* Private Methods*/

std::string Image::getFileExtension(std::string path){
    std::string ext = "";

    for(unsigned int i = path.size() - 1; ; i--){
        if(path[i] == '.') break;

        ext = path[i] + ext;

        if(i == 0) break;
    }

    ASSERT(ext.size() != path.size(), "File " << path << " without extension!");

    return ext;
}


unsigned int Image::loadPNG(){
    std::vector<unsigned char> image; //the raw pixels

    //decode
    unsigned int error;
    error = lodepng_decode32_file(&pixels, &width, &height, path.c_str());

    //if there's an error, display it
    if(error)
        DEBUG("decoder error " << error << ": " << lodepng_error_text(error));

    return error;
}

unsigned int Image::writePNG(std::string filename) {
    unsigned char* png;
    size_t pngsize;

    unsigned error = lodepng_encode32(&png, &pngsize, pixels, width, height);
    if(!error) lodepng_save_file(png, pngsize, filename.c_str());

    /*if there's an error, display it*/
    if(error) DEBUG("error " << error << ": " << lodepng_error_text(error));

    free(png);

    return 0;
}

unsigned int Image::writeBMP(std::string filename) {
    std::vector<unsigned char> bmp;
    int w = width, h = height;

    //3 bytes per pixel used for both input and output.
    int inputChannels = 3;
    int outputChannels = 3;

    //bytes 0-13
    bmp.push_back('B'); bmp.push_back('M'); //0: bfType
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); //2: bfSize; size not yet known for now, filled in later.
    bmp.push_back(0); bmp.push_back(0); //6: bfReserved1
    bmp.push_back(0); bmp.push_back(0); //8: bfReserved2
    bmp.push_back(54 % 256); bmp.push_back(54 / 256); bmp.push_back(0); bmp.push_back(0); //10: bfOffBits (54 header bytes)

    //bytes 14-53
    bmp.push_back(40); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //14: biSize
    bmp.push_back(w % 256); bmp.push_back(w / 256); bmp.push_back(0); bmp.push_back(0); //18: biWidth
    bmp.push_back(h % 256); bmp.push_back(h / 256); bmp.push_back(0); bmp.push_back(0); //22: biHeight
    bmp.push_back(1); bmp.push_back(0); //26: biPlanes
    bmp.push_back(outputChannels * 8); bmp.push_back(0); //28: biBitCount
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //30: biCompression
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //34: biSizeImage
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //38: biXPelsPerMeter
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //42: biYPelsPerMeter
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //46: biClrUsed
    bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //50: biClrImportant

    /*
    Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
    -BMP stores the rows inversed, from bottom to top
    -BMP stores the color channels in BGR instead of RGB order
    -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
    */

    int imagerowbytes = outputChannels * w;
    imagerowbytes = imagerowbytes % 3 == 0 ? imagerowbytes : imagerowbytes + (3 - imagerowbytes % 3);

    for(int y = h - 1; y >= 0; y--) //the rows are stored inversed in bmp
    {
      int c = 0;
      for(int x = 0; x < imagerowbytes; x++)
      {
        if(x < w * outputChannels)
        {
          int inc = c;
          //Convert RGB(A) into BGR(A)
          if(c == 0) inc = 2;
          else if(c == 2) inc = 0;
          bmp.push_back(pixels[inputChannels * (w * y + x / outputChannels) + inc]);
        }
        else bmp.push_back(0);
        c++;
        if(c >= outputChannels) c = 0;
      }
    }

    // Fill in the size
    bmp[2] = bmp.size() % 256;
    bmp[3] = (bmp.size() / 256) % 256;
    bmp[4] = (bmp.size() / 65536) % 256;
    bmp[5] = bmp.size() / 16777216;

    lodepng::save_file(bmp, filename);

    return 0;
}
/* BMP file loader loads a 24-bit bmp file only */

/*
* getint and getshort are help functions to load the bitmap byte by byte
*/
static unsigned int getint(FILE *fp) {
    int c, c1, c2, c3;

    /*  get 4 bytes */
    c = getc(fp);
    c1 = getc(fp);
    c2 = getc(fp);
    c3 = getc(fp);

    return ((unsigned int)c) +
        (((unsigned int)c1) << 8) +
        (((unsigned int)c2) << 16) +
        (((unsigned int)c3) << 24);
}

static unsigned int getshort(FILE *fp){
    int c, c1;

    /* get 2 bytes*/
    c = getc(fp);
    c1 = getc(fp);

    return ((unsigned int)c) + (((unsigned int)c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */

unsigned int Image::loadBMP() {
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */

    /*  make sure the file is there. */
    if ((file = fopen(path.c_str(), "rb")) == NULL) {
        printf("File Not Found : %s\n", path.c_str());
        return 1;
    }

    /*  seek through the bmp header, up to the width height: */
    fseek(file, 18, SEEK_CUR);

    /*  No 100% errorchecking anymore!!! */

    /*  read the width */    width = getint(file);

    /*  read the height */
    height = getint(file);

    /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
    size = width * height * 3;

    /*  read the planes */
    planes = getshort(file);
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", path.c_str(), planes);
        return 1;
    }

    /*  read the bpp */
    bpp = getshort(file);
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", path.c_str(), bpp);
        return 1;
    }

    /*  seek past the rest of the bitmap header. */
    fseek(file, 24, SEEK_CUR);

    /*  read the data.  */
    pixels = (unsigned char *)malloc(size*sizeof(unsigned char));
    if (pixels == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 1;
    }

    if ((i = fread(pixels, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", path.c_str());
        return 1;
    }

    for (i = 0; i<size; i += 3) { /*  reverse all of the colors. (bgr -> rgb) */
        temp = pixels[i];
        pixels[i] = pixels[i + 2];
        pixels[i + 2] = temp;
    }

    fclose(file); /* Close the file and release the filedes */

    /*  we're done. */
    return 0;
}
