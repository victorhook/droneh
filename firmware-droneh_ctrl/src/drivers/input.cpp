#include "drivers/input.h"
#include "sys.h"
#include "state.h"

#define DIFF_SENSITIVITY 5


InputHandler::InputHandler()
:  m_bias({.rx=0, .ry=0, .lx=0, .ly=0}),
   m_idle_limit(10)
{}

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

static void printValues()
{
    Serial.print("LX: "); Serial.print(state.joystick_left.x);
    Serial.print(" LY: "); Serial.print(state.joystick_left.y);
    Serial.print(" RX: "); Serial.print(state.joystick_right.x);
    Serial.print(" RY: "); Serial.println(state.joystick_right.y);
}

void InputHandler::calibrate()
{
    int readings = 10;
    for (int i = 0; i < readings; i++) {
        read();
        m_bias.lx += state.joystick_left.x;
        m_bias.ly += state.joystick_left.y;
        m_bias.rx += state.joystick_right.x;
        m_bias.ry += state.joystick_right.y;
        delay(1);
    }
    m_bias.lx = (m_bias.lx / readings) - 512;
    m_bias.ly = (m_bias.ly / readings) - 512;
    m_bias.rx = (m_bias.rx / readings) - 512;
    m_bias.ry = (m_bias.ry / readings) - 512;
    m_is_calibrated = true;
}

static int inline trim(int value, int bias)
{
    int diff = value - (512 + bias);
    if (abs(diff) < DIFF_SENSITIVITY) {
        value = 512;
    }
    return value;
}

void InputHandler::update()
{
    // Ensure we've callibrated first, to know our bias.
    if (!m_is_calibrated)
        calibrate();

    // Read the joystick values.
    read();

    // Trim the values with the help of the bias. This is to ensure joysticks
    // are always center-aligned (512 mid value).
    state.joystick_left.x = trim(state.joystick_left.x, m_bias.lx);
    state.joystick_left.y = trim(state.joystick_left.y, m_bias.ly);
    state.joystick_right.x = trim(state.joystick_right.x, m_bias.rx);
    state.joystick_right.y = trim(state.joystick_right.y, m_bias.ry);
}

void InputHandler::read()
{
    state.joystick_left.x = 1023 - analogRead(JOYSTICK_LEFT_X);
    state.joystick_left.y = analogRead(JOYSTICK_LEFT_Y);
    state.joystick_right.x = analogRead(JOYSTICK_RIGHT_X);
    state.joystick_right.y = 1023 - analogRead(JOYSTICK_RIGHT_Y);
    // These are active low, so a read of 0 means pressed.
    state.joystick_left.is_pressed = digitalRead(JOYSTICK_LEFT_SW) == 0;
    state.joystick_right.is_pressed = digitalRead(JOYSTICK_RIGHT_SW) == 0;
}

InputHandler input_handler;