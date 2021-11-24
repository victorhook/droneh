#include "drivers/debugger.h"


static size_t last_time;

Debugger::Debugger(State& state, size_t time_delay)
    : Driver(state), m_time_delay(time_delay)
{

}

bool Debugger::init()
{
    Serial.begin(9600);
    Serial.println("Control started, with debug ON.");
    return true;
}

void Debugger::update()
{
    size_t curr_time = millis();
    size_t d_time = curr_time - last_time;
    if (d_time < m_time_delay) {
        return;
    }

    Serial.println("-- Radio --");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");
    Serial.print("Address: ");

    last_time = curr_time;
}