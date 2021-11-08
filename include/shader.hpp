#ifndef SHADER_HPP
#define SHADER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "image.hpp"


class shader
{
    public:
        shader(const char *vertexFile, const char *fragmentFile);
        shader(const char *vertexFile, const char *geometryFile, const char *fragmentFile);
        shader(std::vector<const char*> computeShaders);
        virtual ~shader();

        void useMain();

        GLint getUniformLocation(const char *name);

        int getInt(GLint location);

        void setMatrix4f(GLint location, const glm::mat4 &mat);
        void setInt(GLint location, int newInt);

        void bindTexture(Image *img, const char *unfi_name, int text);

    protected:
      unsigned int mainShader;
    private:
        unsigned int vertexShader, fragmentShader, geometryShader, computeShader;
};

#endif // SHADER_HPP
