#ifndef RENDER_H
#define RENDER_H
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/ext.hpp"
#include "../include/shader.hpp"
#include "../include/cube.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>


class render
{
    public:
        render(const char *winTitle, unsigned int width, unsigned int height);
        virtual ~render();

        void loadBuffers(float *verticies_buffer, size_t size_vex, int *elements_buffer, size_t size_element);

        void renderTriangle(int start, int amount, shader *shaderProgram);

        void renderBasicTriangle(int start, int amount, shader *shaderProgram);

        void renderCubes(Cube **cubes, int max_cubes, shader *shaderProgram);

        void clear_screen(float r, float g, float b);

        void update();

        int getCloseStatus();

    protected:

    private:
        SDL_Window *__win;
        SDL_GLContext __con;
        SDL_Event e;
        // The vertex array, vertex buffer, and element buffer objects
        unsigned int VAO, VBO, EBO;
};

#endif // RENDER_H
