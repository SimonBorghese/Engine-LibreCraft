#include <iostream>
#include "../include/render.hpp"
#include "../include/shader.hpp"
#include "../include/image.hpp"
#include "../include/cube.hpp"
#include "../include/camera.hpp"
#include "../include/world.hpp"
#include "../include/game.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/ext.hpp"

#define WIDTH 400
#define HEIGHT 400

#define FPS_TARGET 60



int VIEWDIST = 20;


using namespace std;

shader *mainShader;
render *mainRender;
Image *top, *bottom, *left_t, *right_t, *front, *back;

GLint wire_loc, projection_loc, view_loc, model_loc, posX_loc, posZ_loc, fogdist_loc;
glm::vec3 *pos, *forward_pos, *trueForward;

uint32_t frameStartTime, frameEndTime;

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

float testVertex[] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

};

Camera *cam;

World *world;

int elements[] = {};

const uint8_t *keymap;

SDL_Event e;

VertexArr baseVAO;

float *testVerticies, *output;
int outputsize;

glm::mat4 model, view, projection;

int x1,y1_p,z1, x2,y2,z2;

void initalizeClasses(){
  // Initalize the renderer and main shader
  mainRender = new render("UwU Minecraft", WIDTH, HEIGHT);
  mainShader = new shader("shaders/vertex.glsl", "shaders/fragment.glsl");

  // Initalize the main images
  bottom = new Image("textures/dirt.jpg", GL_TEXTURE0, GL_RGB,0);
  top = new Image("textures/sand.jpg", GL_TEXTURE1, GL_RGB,0);
  left_t = new Image("textures/sidedirt.jpg", GL_TEXTURE2, GL_RGB, 1);
  right_t = new Image("textures/sidedirt.jpg", GL_TEXTURE3, GL_RGB, 1);
  front = new Image("textures/sidedirt.jpg", GL_TEXTURE4, GL_RGB, 0);
  back = new Image("textures/sidedirt.jpg", GL_TEXTURE5, GL_RGB, 0);
  // Make the camera
  cam = new Camera(glm::vec3(0, 0, 0), 0.005f);
  
  // Make the matricies
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
}

float **chunks;

void makeWorld(){
  // Create the world object
  world = new World();
  output = world->generateAVao(vertices, &outputsize, 0, 3, sizeof(vertices), 6, -8,0,-8 ,8,255,8);
}

void loadBuffers(){
  mainRender->loadBuffers(&baseVAO, output,outputsize*sizeof(float), elements, sizeof(elements));
    
  projection = glm::perspective(glm::radians(90.0f), (float) WIDTH/HEIGHT, 0.1f, (float) VIEWDIST);

  projection_loc = mainShader->getUniformLocation("projection");
  view_loc = mainShader->getUniformLocation("view");
  model_loc = mainShader->getUniformLocation("model");

  wire_loc = mainShader->getUniformLocation("useWire");

  fogdist_loc = mainShader->getUniformLocation("fogdist");
  posX_loc = mainShader->getUniformLocation("posX");
  posZ_loc = mainShader->getUniformLocation("posZ");

  pos = new glm::vec3;
  forward_pos = new glm::vec3;
  trueForward = new glm::vec3;



  mainShader->useMain();
  mainRender->bindCurrentVAO(&baseVAO);

  mainShader->setMatrix4f(projection_loc, projection);

  mainShader->bindTexture(back, "text0", 5); // back
  mainShader->bindTexture(front, "text1", 4); // front
  mainShader->bindTexture(left_t, "text2", 2); // left
  mainShader->bindTexture(right_t, "text3", 3); // right
  mainShader->bindTexture(bottom, "text4", 0); // bottom
  mainShader->bindTexture(top, "text5", 1);

  mainShader->setMatrix4f(model_loc, model);

  //mainShader->setInt(wire_loc, 1);
}

unsigned char *stuffdata;

int main()
{
    initalizeClasses();

    makeWorld();
  
    loadBuffers();

    uint32_t startRender, endRender, deltaTime = 0, lastFrame;
    uint32_t waitTime = 1000/FPS_TARGET;
    uint32_t fps_c = 0;

    endRender = SDL_GetTicks();
    lastFrame = endRender;
    
    int posChanged = 1;


    while (!(e.type == SDL_QUIT)){
      *pos = cam->getPos();
      *forward_pos = cam->getForward();
      *trueForward = *pos+(*forward_pos*2.0f);
      // Uncomment to get fake-physics
      //if (!world->getBlockState(pos->x, pos->y-2, pos->z)){
          
      //}
      if (posChanged){
        cam->setPos(glm::vec3(pos->x, world->getBlockHeight(round(pos->x),  round(pos->z)) + 2, pos->z));

        *pos = cam->getPos();

        if (world->getBlockState(pos->x, pos->y-1, pos->z)){
          cam->setPos(glm::vec3(pos->x, pos->y+1, pos->z));
        }
        if (world->getBlockState(pos->x, pos->y, pos->z)){
          cam->setPos(glm::vec3(pos->x, pos->y+2, pos->z));
        }
        posChanged = 0;
      }


      startRender = SDL_GetTicks();
      SDL_PollEvent(&e);
      cam->setDeltaTime((float) deltaTime);

        //begin input code
      keymap = SDL_GetKeyboardState(NULL);
      if (keymap[SDL_SCANCODE_W]){
        cam->moveForward();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_S]){
        cam->moveBackward();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_A]){
        cam->strafeLeft();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_D]){
        cam->strafeRight();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_UP]){
        cam->moveUp();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_DOWN]){
        cam->moveDown();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_LEFT]){
        cam->turnLeft();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_RIGHT]){
        cam->turnRight();
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_Z]){
        world->createBlock(trueForward->x,trueForward->y,trueForward->z, 1);
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_X]){
        world->destroyBlock(trueForward->x,trueForward->y,trueForward->z);
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_SPACE]){
        *pos = cam->getPos();
        cam->setPos(glm::vec3(pos->x, pos->y+2, pos->z));
        posChanged = 1;
      }
      

      mainRender->clear_screen(0.62f, 0.99f, 1.0f);

      x1 = (int) round(pos->x) -VIEWDIST;
      //y1 = (int) round(pos->x) -VIEWDIST;
      y1_p = 0;
      z1 = (int) round(pos->z) -VIEWDIST;

      x2 = (int) round(pos->x) +VIEWDIST;
      y2 = (int) round(pos->y) +VIEWDIST;
      z2 = (int) round(pos->z) +VIEWDIST;
      //printf("Rendering to: %d %d %d :: %d %d %d\n", x1,y1_p,z1,x2,y2,z2);
      output = world->generateAVao(vertices, &outputsize, 0, 3, sizeof(vertices), 6, x1,y1_p,z1,x2,y2,z2);

      //mainRender->overwriteVBOBuffer(&baseVAO, 0, outputsize*sizeof(float), output);
      mainRender->loadBuffers(&baseVAO, output,outputsize*sizeof(float), elements, sizeof(elements));

      cam->setView((&view));
      mainShader->setMatrix4f(view_loc, view);
      mainShader->setFloat(posX_loc, pos->x);
      mainShader->setFloat(posZ_loc, pos->z);
      mainShader->setFloat(fogdist_loc, (float) VIEWDIST-5);
      
      


      //mainShader->setInt(wire_loc, 0);
      mainRender->renderBasicTriangle(0, outputsize);

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