#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"

class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

protected:
    InputManager();

public:
    virtual ~InputManager();

    void initialize(GLFWwindow* window);

    void pollJoystick();

private:
    int joystick1AxesCount;
    const float* joystick1Axes;
    bool joystick1Directions[4];
    int joystick1ButtonCount;
    const unsigned char* joystick1Buttons;

    int joystick2AxesCount;
    const float* joystick2Axes;
    bool joystick2Directions[4];
    int joystick2ButtonCount;
    const unsigned char* joystick2Buttons;
};