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

#define VIEWDIST 50

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

    projection = glm::perspective(glm::radians(90.0f), (float) WIDTH/HEIGHT, 0.1f, (float) VIEWDIST);


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


    uint32_t startStat, endStat;

    endRender = SDL_GetTicks();
    lastFrame = endRender;
    glm::vec3 pos;
    
    GLint projection_loc = mainShader->getUniformLocation("projection");
    GLint view_loc = mainShader->getUniformLocation("view");
    GLint model_loc = mainShader->getUniformLocation("model");


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
      mainShader->setMatrix4f(view_loc, view);
      mainShader->setMatrix4f(projection_loc, projection);


      




      //cubes[0]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
      //startStat = SDL_GetTicks();
      for (int x = (int) pos.x + VIEWDIST/2; x > (int) pos.x - VIEWDIST/2; x--){
        for (int y = (int) pos.y + VIEWDIST/2; y>(int) pos.y - VIEWDIST/2; y--){
          for (int z = (int) pos.z + VIEWDIST/2; z>(int) pos.z - VIEWDIST/2; z--){
            //printf("Current Pos: %f %f %f\n", pos.x, pos.y, pos.z);
            //printf("Checking: %d %d %d\n",x,y,z);
            if (world->getRenderState(x,y,z)){
              if (abs(pos.x - x) < VIEWDIST && abs(pos.y - y) < VIEWDIST && abs(pos.z - z) < VIEWDIST){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f + x, 0.0f+ y, 0.0f + z));
                mainShader->setMatrix4f(model_loc, model);

                mainRender->renderBasicTriangle(0, 36, mainShader);
              }
            }
          }
        }
      }
      //endStat = SDL_GetTicks();
      //printf("Full render time: %d\n", endStat - startStat);

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