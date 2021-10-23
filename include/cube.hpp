#ifndef CUBE_H
#define CUBE_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Cube{
  public:
    Cube(glm::vec3 position, glm::vec3 scale);
    virtual ~Cube();
    
    void move(glm::vec3 newPos);
    void rotate(glm::vec3 newRot, float radians);
    void scale(glm::vec3 newScl);

    glm::mat4* getMat();
  private:
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scl;

    glm::mat4 *transform_obj;
  
};

#endif