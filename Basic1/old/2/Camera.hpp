//
//  Camera.hpp
//  Basic1
//
//  Created by Никита Исаенко on 07.01.2021.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN, FOCUS_LOST, FOCUS_GAIN };

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    glm::vec3 Position, Front, Up, Right, WorldUp;
    float Yaw = -90.f, Pitch = 0.f;
    float MouseSensitivity = 0.1f, Zoom = 45.f;
    float NormalSpeed = 2.5f, ShiftSpeed = 10.f, MovementSpeed = NormalSpeed;
    bool hasFocus;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.f);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

#endif /* Camera_hpp */
