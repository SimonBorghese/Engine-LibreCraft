#ifndef IMAGE_H
#define IMAGE_H
#include "stb_image.h"
#include "glad/glad.h"
#include <stdio.h>

class Image{
  public:
    Image(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip);
    virtual ~Image();

    GLenum getImageNum();
    unsigned int getTexture();

  private:
    unsigned int texture;
    int width, height, nrChannels;
    unsigned char *data;
    GLenum image_count;
  
};

#endif