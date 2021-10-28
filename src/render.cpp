#include "../include/render.hpp"


render::render(const char *winTitle, unsigned int width, unsigned int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "ERROR INITALIZING SDL: " << SDL_GetError();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  

    __win = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    __con = SDL_GL_CreateContext(__win);
    SDL_GL_MakeCurrent(__win, __con);

    gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

    glViewport(0,0,width, height);

    glEnable(GL_DEPTH_TEST);


    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    glEnable(GL_MULTISAMPLE);


    //glEnable(GL_CULL_FACE);  
    //glCullFace(GL_BACK);
}

render::~render()
{
    SDL_GL_DeleteContext(__con);
    SDL_DestroyWindow(__win);
    SDL_Quit();
}

void render::loadBuffers(VertexArr *mainArr, float *verticies_buffer, size_t size_vex, int *elements_buffer, size_t size_element){

    glGenVertexArrays(1, &mainArr->VAO);
    glGenBuffers(1, &mainArr->VBO);
    glGenBuffers(1, &mainArr->EBO);

    glBindVertexArray(mainArr->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mainArr->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainArr->EBO);

    glBufferData(GL_ARRAY_BUFFER, size_vex, verticies_buffer, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_element, elements_buffer, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 5));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

}

void render::renderTriangle(int start, int amount){
    glDrawElements(GL_TRIANGLES, amount, GL_UNSIGNED_INT, (void*) start);
}

void render::renderCubes(Cube **cubes, int max_cubes, shader *shaderProgram){
  GLint *model_lc = (GLint*) malloc(sizeof(GLint));
  *model_lc = shaderProgram->getUniformLocation("model");
  shaderProgram->useMain();

  glBindVertexArray(VAO);
  for (int c = 0; (c < max_cubes); c++){

    if (cubes[c] != NULL){
      shaderProgram->setMatrix4f(*model_lc, *cubes[c]->getMat());
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    else{
      c = max_cubes;
      printf("Warning: Invalid cube caught, exiting loop\n");
    }
  }
  delete model_lc;
}

void render::renderBasicTriangle(int start, int amount){
  glDrawArrays(GL_TRIANGLES, start, amount);
  
}

void render::clear_screen(float r, float g, float b){
    glClearColor(r,g,b,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render::update(){
    SDL_GL_SwapWindow(__win);
    SDL_UpdateWindowSurface(__win);
}

int render::getCloseStatus(){
  SDL_PollEvent(&e);
  return (e.type == SDL_QUIT);
}

void render::bindCurrentVAO(VertexArr *arr){
  glBindVertexArray(arr->VAO);
}