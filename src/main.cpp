#include <iostream>
#include "../include/render.hpp"
#include "../include/shader.hpp"
#include "../include/image.hpp"
#include "../include/cube.hpp"
#include "../include/camera.hpp"
#include "../include/world.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#define WIDTH 400
#define HEIGHT 400

#define FPS 60

using namespace std;

shader *mainShader;
render *mainRender;
Image *top, *bottom, *left_t, *right_t, *front, *back;


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 2.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 2.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2.0f,

     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 3.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 3.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 3.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 3.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 3.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 3.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5.0f
};

Camera *cam;

World *world;

int elements[] = {};

const uint8_t *keymap;

SDL_Event e;

void handleInput();

int main()
{
    mainRender = new render("UwU Minecraft", WIDTH, HEIGHT);
    mainShader = new shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    bottom = new Image("dirt.jpg", GL_TEXTURE0, GL_RGB,0);
    top = new Image("sand.jpg", GL_TEXTURE1, GL_RGB,0);
    left_t = new Image("sidedirt.jpg", GL_TEXTURE2, GL_RGB, 1);
    right_t = new Image("sidedirt.jpg", GL_TEXTURE3, GL_RGB, 1);
    front = new Image("sidedirt.jpg", GL_TEXTURE4, GL_RGB, 0);
    back = new Image("sidedirt.jpg", GL_TEXTURE5, GL_RGB, 0);
    /*
    bottom
    top
    left
    right
    front
    back
    */
    mainRender->loadBuffers(vertices, sizeof(vertices), elements, sizeof(elements));

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(90.0f), (float) WIDTH/HEIGHT, 0.1f, 40.0f);


    // Width, height, length
    world = new World(200,30,200, 200,200,200);
    int baseX, baseY, baseZ;
    baseX = world->getWidth()/2;
    baseZ = world->getTall()/2;
    baseY = world->getBlockHeight(baseX, baseZ) + 2;
    cam = new Camera(glm::vec3(baseX, baseY, baseZ), 0.005f, WIDTH, HEIGHT);


    uint32_t startRender, endRender, deltaTime, lastFrame;
    uint32_t waitTime = 1000/FPS;
    uint32_t fps_c = 0;



    endRender = SDL_GetTicks();
    lastFrame = endRender;
    glm::vec3 pos;
    while (!(e.type == SDL_QUIT)){
      pos = cam->getPos();
      cam->setPos(glm::vec3(pos.x, world->getBlockHeight(floor(pos.x), floor(pos.z)) + 2, pos.z));

      startRender = SDL_GetTicks();
      SDL_PollEvent(&e);
      keymap = SDL_GetKeyboardState(NULL);
      cam->setDeltaTime((float) deltaTime);
      if (keymap[SDL_SCANCODE_W]){
        cam->moveForward();
      }
      if (keymap[SDL_SCANCODE_S]){
        cam->moveBackward();
      }
      if (keymap[SDL_SCANCODE_A]){
        cam->strafeLeft();
      }
      if (keymap[SDL_SCANCODE_D]){
        cam->strafeRight();
      }
      if (keymap[SDL_SCANCODE_UP]){
        cam->moveUp();
      }
      if (keymap[SDL_SCANCODE_DOWN]){
        cam->moveDown();
      }
      if (keymap[SDL_SCANCODE_LEFT]){
        cam->turnLeft();
      }
      if (keymap[SDL_SCANCODE_RIGHT]){
        cam->turnRight();
      }

      mainRender->clear_screen(0.8f, 1.0f, 1.0f);
            
      /*
    bottom 0 
    top 1
    left 2
    right 3
    front 4
    back 5
    */
      
      mainShader->bindTexture(back, "text0", 5); // back
      mainShader->bindTexture(front, "text1", 4); // front
      mainShader->bindTexture(left_t, "text2", 2); // left
      mainShader->bindTexture(right_t, "text3", 3); // right
      mainShader->bindTexture(bottom, "text4", 0); // bottom
      mainShader->bindTexture(top, "text5", 1);


      cam->setView((&view));
      mainShader->setMatrix4f("view", view);

      mainShader->setMatrix4f("projection", projection);


      //cubes[0]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);

      for (int x = world->getMaxWidth(); x>0; x--){
        for (int y = world->getMaxHeight(); y>0; y--){
          for (int z = world->getMaxTall(); z>0; z--){
            if (world->getRenderState(x,y,z)){
              model = glm::mat4(1.0f);
              model = glm::translate(model, glm::vec3(0.0f + x, 0.0f+ y, 0.0f + z));
              mainShader->setMatrix4f("model", model);

              mainRender->renderBasicTriangle(0, 36, mainShader);
            }
          }
        }
      }

      //mainRender->renderCubes(cubes, cube_count, mainShader); 

      mainRender->update();
      endRender = SDL_GetTicks();
      deltaTime = endRender - startRender;

      fps_c++;
      if ((SDL_GetTicks() - lastFrame) >=1000){
        printf("\rFPS: %d  ", fps_c);
        fflush(stdout);
        fps_c = 0;
        lastFrame = SDL_GetTicks();
      }
      if (deltaTime <= waitTime){
        //SDL_Delay(waitTime - deltaTime);
      }


    }

    return 0;
}