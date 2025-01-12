#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::initialize(GLFWwindow* window)
{
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        spdlog::info("Joystick 1 is connected: {}", glfwGetJoystickName(GLFW_JOYSTICK_1));

        joystick1Axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &joystick1AxesCount);
        joystick1Buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &joystick1ButtonCount);

        joystick1Connected = true;

    } else {
        spdlog::info("No joystick connected!");
        joystick1Connected = false;
    }

    if (glfwJoystickPresent(GLFW_JOYSTICK_2)) {
        spdlog::info("Joystick 2 is connected: {}", glfwGetJoystickName(GLFW_JOYSTICK_2));

        joystick2Axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &joystick2AxesCount);
        joystick2Buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &joystick2ButtonCount);

        joystick2Connected = true;

    } else {
        spdlog::info("No joystick connected!");
        joystick2Connected = false;
    }
}

void InputManager::pollJoystick()
{

    if(joystick1Connected) {
        joystick1Buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &joystick1ButtonCount);

        for (int i = 0; i < joystick1ButtonCount; ++i) {
            if (joystick1Buttons[i] == GLFW_PRESS) {
                //spdlog::info("Button {}: {}", i, joystick1Buttons[i]);
            }
        }

        joystick1Axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &joystick1AxesCount);

        //Left
        joystick1Directions[2] = joystick1Axes[0] < -0.5;
        
        //Right
        joystick1Directions[3] = joystick1Axes[0] > 0.5;

        //Up
        joystick1Directions[0] = joystick1Axes[1] < -0.5;

        //Down
        joystick1Directions[1] = joystick1Axes[1] > 0.5;
    }

    if(joystick2Connected) {
        joystick2Buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &joystick2ButtonCount);

        for (int i = 0; i < joystick2ButtonCount; ++i) {
            if (joystick2Buttons[i] == GLFW_PRESS) {
                //spdlog::info("J2 Button {}: {}", i, joystick2Buttons[i]);
            }
        }

        joystick2Axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &joystick2AxesCount);

        //Left
        joystick2Directions[2] = joystick2Axes[0] < -0.5;

        //Right
        joystick2Directions[3] = joystick2Axes[0] > 0.5;

        //Up
        joystick2Directions[0] = joystick2Axes[1] < -0.5;

        //Down
        joystick2Directions[1] = joystick2Axes[1] > 0.5;
    }

}