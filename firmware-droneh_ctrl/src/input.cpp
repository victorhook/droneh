#include "drivers/input.h"


InputHandler::InputHandler(State* state)
    : Driver(state)
{

}

bool InputHandler::init()
{
    pinMode(JOYSTICK_LEFT_X, INPUT);
    pinMode(JOYSTICK_LEFT_Y, INPUT);
    pinMode(JOYSTICK_LEFT_SW, INPUT_PULLUP);
    pinMode(JOYSTICK_RIGHT_X, INPUT);
    pinMode(JOYSTICK_RIGHT_Y, INPUT);
    pinMode(JOYSTICK_RIGHT_SW, INPUT_PULLUP);
    return true;
}

void InputHandler::update()
{
    m_state->joystick_left.x = analogRead(JOYSTICK_LEFT_X);
    m_state->joystick_left.y = analogRead(JOYSTICK_LEFT_Y);
    m_state->joystick_left.is_pressed = digitalRead(JOYSTICK_LEFT_X);
    m_state->joystick_right.x = analogRead(JOYSTICK_RIGHT_X);
    m_state->joystick_right.y = analogRead(JOYSTICK_RIGHT_Y);
    m_state->joystick_right.is_pressed = digitalRead(JOYSTICK_RIGHT_SW);
}
