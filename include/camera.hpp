#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtx/rotate_vector.hpp"

#define CAM_TURN_SPEED 1000

class Camera{
  public:
    Camera(glm::vec3 newCameraPos, float speed);
    virtual ~Camera();

    void setSpeed(float speed);

    void moveForward();
    void moveBackward();
    
    void moveUp();
    void moveDown();

    void strafeLeft();
    void strafeRight();

    void turnLeft();
    void turnRight();

    void setDeltaTime(float newTime);

    void setView(glm::mat4 *view);
    
    glm::vec3 getPos();
    glm::vec3 getForward();
    void setPos(glm::vec3 newPos);

    float getPitch();
    float getYaw();


  private:
    float cam_speed;

    //Basic Camera values
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;

    // Some Camera values
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    // Const values
    glm::vec3 up;
    glm::vec3 right;

    uint32_t t_deltaTime;

    //float yaw;
    //float pitch;

    glm::mat4 *view_mat;

    float yaw_t;	// -90
    float pitch_t; 

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

};

#endif