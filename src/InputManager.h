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

public:
    bool joystick1Connected;
    bool joystick1Directions[4];
    int joystick1ButtonCount;
    const unsigned char* joystick1Buttons;

    bool joystick2Connected;
    bool joystick2Directions[4];
    int joystick2ButtonCount;
    const unsigned char* joystick2Buttons;

private:
    int joystick1AxesCount;
    const float* joystick1Axes;
    
    int joystick2AxesCount;
    const float* joystick2Axes;
};