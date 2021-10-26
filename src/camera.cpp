#include "../include/camera.hpp"

Camera::Camera(glm::vec3 newCameraPos, float speed, int WIN_WIDTH, int WIN_HEIGHT) : cameraPos(newCameraPos), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),  up(glm::vec3(0.0f, 1.0f, 0.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)), cam_speed(speed), yaw_t(-90.0f), pitch_t(0.0f), t_deltaTime(1.0f){

}
Camera::~Camera(){

}

void Camera::setSpeed(float speed){
  cam_speed = speed;
}

void Camera::moveForward(){
  float *tempKeep = (float*) malloc(sizeof(float));
  *tempKeep = cameraPos.y;
  cameraPos += cam_speed * cameraFront * (float) t_deltaTime;
  // Uncomment to make FPS Cam
  cameraPos.y = *tempKeep;
  delete tempKeep;
}
void Camera::moveBackward(){
  float *tempKeep = (float*) malloc(sizeof(float));
  *tempKeep = cameraPos.y;
  cameraPos -= cam_speed * cameraFront * (float) t_deltaTime;
  cameraPos.y = *tempKeep;

  delete tempKeep;
}

void Camera::moveUp(){

  pitch_t += CAM_TURN_SPEED * cam_speed;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch_t > 89.0f)
        pitch_t = 89.0f;
    if (pitch_t < -89.0f)
        pitch_t = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    front.y = sin(glm::radians(pitch_t));
    front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
    cameraFront = glm::normalize(front);
}

void Camera::moveDown(){

  pitch_t -= CAM_TURN_SPEED * cam_speed;

  if (pitch_t > 89.0f)
      pitch_t = 89.0f;
  if (pitch_t < -89.0f)
      pitch_t = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  front.y = sin(glm::radians(pitch_t));
  front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  cameraFront = glm::normalize(front);
}

void Camera::strafeLeft(){
  cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed * (float) t_deltaTime;
}
void Camera::strafeRight(){
  cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cam_speed  * (float) t_deltaTime;
}

void Camera::turnLeft(){

  yaw_t -= CAM_TURN_SPEED * cam_speed;
  //pitch_t += 1;

  if (pitch_t > 89.0f)
      pitch_t = 89.0f;
  if (pitch_t < -89.0f)
      pitch_t = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  front.y = sin(glm::radians(pitch_t));
  front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  cameraFront = glm::normalize(front);

}
void Camera::turnRight(){

  yaw_t += CAM_TURN_SPEED * cam_speed;
  //pitch_t += 1;

  if (pitch_t > 89.0f)
      pitch_t = 89.0f;
  if (pitch_t < -89.0f)
      pitch_t = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  front.y = sin(glm::radians(pitch_t));
  front.z = sin(glm::radians(yaw_t)) * cos(glm::radians(pitch_t));
  cameraFront = glm::normalize(front);
}

void Camera::setView(glm::mat4 *view){
  view_mat = view;
  *view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::setDeltaTime(float newTime){
  t_deltaTime = newTime;
}


glm::vec3 Camera::getPos(){
  return cameraPos;
}
void Camera::setPos(glm::vec3 newPos){
  cameraPos = newPos;
}