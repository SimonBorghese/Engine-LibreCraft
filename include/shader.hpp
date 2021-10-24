#ifndef SHADER_HPP
#define SHADER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "image.hpp"


class shader
{
    public:
        shader(const char *vertexFile, const char *fragmentFile);
        virtual ~shader();

        void useMain();

        GLint getUniformLocation(const char *name);
        void setMatrix4f(GLint location, const glm::mat4 &mat);

        void bindTexture(Image *img, const char *unfi_name, int text);

    protected:

    private:
        unsigned int vertexShader, fragmentShader, mainShader;
};

#endif // SHADER_HPP
