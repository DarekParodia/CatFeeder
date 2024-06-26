#include "stepper.h"

float speed = 1.0f; // revolutions per second

// some temporary shit
long last_step_time;
float time_per_step = 0;
int stepsToGo = 0;
int stepsDone = 0;

void initStepper(stepper_t *stepper, pin_t *step_pin, pin_t *dir_pin, pin_t *en_pin)
{
    // setup struct
    stepper->step_pin = step_pin;
    stepper->dir_pin = dir_pin;
    stepper->en_pin = en_pin;

    // setup pins
    setPin(step_pin, HIGH);
    setPin(dir_pin, DEF_DIRERCTION);
    disableStepper(stepper);
}

void step(stepper_t *stepper)
{
    long now = micros();

    if (now - last_step_time >= time_per_step) // if time between last step is lower then it should don't do anything
    {
        setPin(stepper->step_pin, LOW);
        last_step_time = now;
        stepsDone++;
        setPin(stepper->step_pin, HIGH);
    }
}

void setSpeed(float s) // i don't think i would need this in the future but it's here anyway; Edit: yueah i ussed id
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
    setPin(stepper->en_pin, HIGH);
}

void enableStepper(stepper_t *stepper)
{
    setPin(stepper->en_pin, LOW);
}
void stepperForward(stepper_t *stepper)
{
    setPin(stepper->dir_pin, DEF_DIRERCTION);
}
void stepperBackward(stepper_t *stepper)
{
    setPin(stepper->dir_pin, !DEF_DIRERCTION);
}