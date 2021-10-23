#include "../include/cube.hpp"


Cube::Cube(glm::vec3 position, glm::vec3 scale){
  transform_obj = new glm::mat4(1.0f);

  *transform_obj = glm::translate(*transform_obj, position);
  *transform_obj = glm::scale(*transform_obj, scale);
}
Cube::~Cube(){
  free(transform_obj);
}
    
void Cube::move(glm::vec3 newPos){
  *transform_obj = glm::translate(*transform_obj, newPos);
}
void Cube::rotate(glm::vec3 newRot, float radians){
  *transform_obj = glm::rotate(*transform_obj, glm::radians(radians), newRot);
}
void Cube::scale(glm::vec3 newScl){
  *transform_obj = glm::scale(*transform_obj, newScl);
}

glm::mat4* Cube::getMat(){
  return transform_obj;
}