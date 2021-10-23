#define STB_IMAGE_IMPLEMENTATION
#include "../include/image.hpp"

Image::Image(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip){
  glGenTextures(1, &texture);
  glActiveTexture(texture_num);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(flip);
  data = stbi_load(fileName, &width, &height, &nrChannels, 0);
  if (data){
    glTexImage2D(GL_TEXTURE_2D, 0, RGB_TYPE, width, height, 0, RGB_TYPE, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    printf("Failed to load image: %s\n", fileName);
  }
  stbi_image_free(data);
  image_count = texture_num;
}

Image::~Image(){

}

GLenum Image::getImageNum(){
  return image_count;
}

unsigned int Image::getTexture(){
  return texture;
}