#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "../include/render.hpp"
#include "../include/world.hpp"
#include "../include/camera.hpp"
#include "../include/shader.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/glm/gtx/transform.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"

/*
The game code will contain all functions that are needed for the game

It is not designed to replace the main file
but it will simplify the process and make it cleaner
*/

class Game{
  public:
    Game(render *rend, Camera *cam, World *wrd);
    virtual ~Game();

    void handleInput();

  private:
    render *m_render;
    Camera *m_cam;
    World *m_world;
    Image *i_top, *i_bottom, *i_left, *i_right, *i_front, *i_back;
    

    uint32_t WIDTH, HEIGHT, VIEWDIST, FPS_TARGET;
};


#endif