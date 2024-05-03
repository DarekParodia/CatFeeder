#ifndef STEPPER_H
#define STEPPER_H

#define MICROSTEPPING 16
#define STEPS_PER_REVOLUTION 200
#define ACTUAL_STEPS_PER_REVOLUTION (MICROSTEPPING * STEPS_PER_REVOLUTION)

#include <avr/io.h>
#include "general.h"

struct stepper_t
{
    volatile uint8_t *port;
    uint8_t pin;
    volatile uint8_t *en_port;
    uint8_t en_pin;
};
typedef struct stepper_t stepper_t;

void initStepper(stepper_t *stepper, volatile uint8_t *port, uint8_t pin, volatile uint8_t *en_port, uint8_t en_pin);
void step(stepper_t *stepper);
void rotateByDegrees(stepper_t *stepper, float degrees);
void setSpeed(int s);

#endif // STEPPER_H