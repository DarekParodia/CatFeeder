#include "stepper.h"

float speed = 1.1f; // revolutions per second

// some temporary shit
long last_step_time;
float time_per_step = 0;
int stepsToGo = 0;
int stepsDone = 0;

void initStepper(stepper_t *stepper, volatile uint8_t *port, uint8_t pin, volatile uint8_t *en_port, uint8_t en_pin)
{
    // setup struct
    stepper->port = port;
    stepper->pin = pin;
    stepper->en_port = en_port;
    stepper->en_pin = en_pin;

    // setup pins
    setPin(port, pin, HIGH);
    disableStepper(stepper);
}

void step(stepper_t *stepper)
{
    long now = micros();

    if (now - last_step_time >= time_per_step) // if time between last step is lower then it should don't do anything
    {
        setPin(stepper->port, stepper->pin, LOW);
        last_step_time = now;
        stepsDone++;
        setPin(stepper->port, stepper->pin, HIGH);
    }
}

void setSpeed(int s) // i don't think i would need this in the future but it's here anyway
{
    speed = s;
}

void rotateByDegrees(stepper_t *stepper, float degrees)
{
    // some calculations
    stepsToGo = degrees / 360 * ACTUAL_STEPS_PER_REVOLUTION;
    stepsDone = 0;
    time_per_step = 1000000.0f / (float)(speed * ACTUAL_STEPS_PER_REVOLUTION);
    last_step_time = micros();

    enableStepper(stepper);
    while (stepsDone < stepsToGo)
    {
        step(stepper);
    }
    disableStepper(stepper);
}

void disableStepper(stepper_t *stepper)
{
    setPin(stepper->en_port, stepper->en_pin, HIGH);
}

void enableStepper(stepper_t *stepper)
{
    setPin(stepper->en_port, stepper->en_pin, LOW);
}