#include "motor.h"

void motorInit(void) {
    gpioConfig(DIR1, GPIO_OUTPUT);
    gpioConfig(DIR2, GPIO_OUTPUT);
    gpioConfig(ENABLE, GPIO_OUTPUT);
    
    // Asegurar que el motor esté apagado al inicio
    desactivarMotor();
}

void activarMotor(void) {
    gpioWrite(ENABLE, ON);
    gpioWrite(DIR1, ON);
    gpioWrite(DIR2, OFF);
}

void desactivarMotor(void) {
    gpioWrite(ENABLE, OFF);
}