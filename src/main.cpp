#include <iostream>
#include "../include/render.hpp"
#include "../include/shader.hpp"
#include "../include/image.hpp"
#include "../include/cube.hpp"
#include "../include/camera.hpp"
#include "../include/world.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/glm/gtx/transform.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/ext.hpp"

#define WIDTH 300
#define HEIGHT 300

#define NO_FPS_OPTIMIZATION
#define FPS_TARGET 30

int VIEWDIST = 30;


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

VertexArr baseVAO;

int main()
{
  
    mainRender = new render("UwU Minecraft", WIDTH, HEIGHT);
    mainShader = new shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    bottom = new Image("textures/dirt.jpg", GL_TEXTURE0, GL_RGB,0);
    top = new Image("textures/sand.jpg", GL_TEXTURE1, GL_RGB,0);
    left_t = new Image("textures/sidedirt.jpg", GL_TEXTURE2, GL_RGB, 1);
    right_t = new Image("textures/sidedirt.jpg", GL_TEXTURE3, GL_RGB, 1);
    front = new Image("textures/sidedirt.jpg", GL_TEXTURE4, GL_RGB, 0);
    back = new Image("textures/sidedirt.jpg", GL_TEXTURE5, GL_RGB, 0);
    /*
    bottom
    top
    left
    right
    front
    back
    */
    

    mainRender->loadBuffers(&baseVAO, vertices, sizeof(vertices), elements, sizeof(elements));

    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(90.0f), (float) WIDTH/HEIGHT, 0.1f, (float) VIEWDIST);


    // Width, height, length
    world = new World();
    int baseX, baseY, baseZ;
    baseX = 0;
    baseZ = 0;
    baseY = 0;
    cam = new Camera(glm::vec3(baseX, baseY, baseZ), 0.005f, WIDTH, HEIGHT);


    uint32_t startRender, endRender, deltaTime, lastFrame;
    uint32_t waitTime = 1000/FPS_TARGET;
    uint32_t fps_c = 0;


    uint32_t startStat, endStat;

    endRender = SDL_GetTicks();
    lastFrame = endRender;
    glm::vec3 pos, forward, trueForward;
    
    GLint projection_loc = mainShader->getUniformLocation("projection");
    GLint view_loc = mainShader->getUniformLocation("view");
    GLint model_loc = mainShader->getUniformLocation("model");

    GLint wire_loc = mainShader->getUniformLocation("useWire");

    
    if (GLAD_GL_ARB_compute_shader){
      printf("Compute Shader available\n");
    }

    int posChanged = 1;


    mainShader->useMain();
    mainRender->bindCurrentVAO(&baseVAO);
    while (!(e.type == SDL_QUIT)){

      
      
      pos = cam->getPos();
      forward = cam->getForward();
      trueForward = pos+(forward*2.0f);
      // Uncomment to get fake-physics
      if (!world->getBlockState(pos.x, pos.y-2, pos.z)){
          cam->setPos(glm::vec3(pos.x, pos.y-0.5, pos.z));
      }
      if (posChanged){
        //cam->setPos(glm::vec3(pos.x, world->getBlockHeight(round(pos.x),  round(pos.z)) + 2, pos.z));
        if (world->getBlockState(pos.x, pos.y-1, pos.z)){
          cam->setPos(glm::vec3(pos.x, pos.y+1, pos.z));
        }
        if (world->getBlockState(pos.x, pos.y, pos.z)){
          cam->setPos(glm::vec3(pos.x, pos.y+2, pos.z));
        }
        posChanged = 0;
      }
      
      //if (cam->getYaw() > 0.0){
      //  cam->setPos(glm::vec3(pos.x, world->getBlockHeight(ceil(pos.x), ceil(pos.z)) + 2, pos.z));
      //}
      //else{
      //  cam->setPos(glm::vec3(pos.x, world->getBlockHeight(floor(pos.x), floor(pos.z)) + 2, pos.z));
      //}

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
        world->createBlock(trueForward.x,trueForward.y,trueForward.z, 1);
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_X]){
        world->destroyBlock(trueForward.x,trueForward.y,trueForward.z);
        posChanged = 1;
      }
      if (keymap[SDL_SCANCODE_SPACE]){
        pos = cam->getPos();
        cam->setPos(glm::vec3(pos.x, pos.y+2, pos.z));
        posChanged = 1;
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
      



      mainShader->setInt(wire_loc, 0);

      //cubes[0]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
      //startStat = SDL_GetTicks();
      int draws = 0;
      for (int x = (int) pos.x + VIEWDIST/2; x > (int) pos.x - VIEWDIST/2; x--){
        for (int z = (int) pos.z + VIEWDIST/2; z>(int) pos.z - VIEWDIST/2; z--){
          int highesty = pos.y-(VIEWDIST/2) - 1;
          for (int y = (int) pos.y + VIEWDIST/2; y>(int) pos.y - VIEWDIST/2; y--){


            //printf("Current Pos: %f %f %f\n", pos.x, pos.y, pos.z);
            if (world->isWorldOrUser(x,y,z)){
            if (world->getBlockState(x,y,z) && y > highesty) {
              highesty = y;
              if (abs(pos.x - x) < VIEWDIST && abs(pos.y - y) < VIEWDIST && abs(pos.z - z) < VIEWDIST){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f + x, 0.0f+ y, 0.0f + z));
                mainShader->setMatrix4f(model_loc, model);

                mainRender->renderBasicTriangle(0, 36);
                draws++;
              }
            }
            }
            else{
              if (world->getBlockState(x,y,z)){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f + x, 0.0f+ y, 0.0f + z));
                mainShader->setMatrix4f(model_loc, model);

                mainRender->renderBasicTriangle(0, 36);
              }
            }
          }
        }
      }

      //printf("Drew: %d\n", draws);
      draws = 0;
      


      mainShader->setInt(wire_loc, 1);
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      model = glm::mat4(1.0f);
      
      model = glm::translate(model, glm::vec3((int) trueForward.x,+ (int) trueForward.y, (int) trueForward.z));
  

      mainShader->setMatrix4f(model_loc, model);

      mainRender->renderBasicTriangle(0, 36);
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

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

        #ifndef NO_FPS_OPTIMIZATION
        if (fps_c < FPS_TARGET){
          VIEWDIST--;
        }
        if (fps_c >= FPS_TARGET){
          VIEWDIST++;
        }
        #endif
        
        fps_c = 0;
        lastFrame = SDL_GetTicks();
      }
      if (deltaTime <= waitTime){
        SDL_Delay(waitTime - deltaTime);
      }


    }

    return 0;
}