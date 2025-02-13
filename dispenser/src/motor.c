#include "motor.h"

void motorInit(void) {
    // Configurar pines de dirección
    gpioConfig(DIR1, GPIO_OUTPUT);
    gpioConfig(DIR2, GPIO_OUTPUT);
    
    // Configurar PWM
    pwmConfig(0, PWM_ENABLE);
    pwmConfig(ENABLE, PWM_ENABLE_OUTPUT);
    
    desactivarMotor();
}

void activarMotor(void) {
    gpioWrite(DIR1, ON);
    gpioWrite(DIR2, OFF);
    pwmWrite(ENABLE, VELOCIDAD_NORMAL);
}

void motorReversa(void) {
    gpioWrite(DIR1, OFF);
    gpioWrite(DIR2, ON);
    pwmWrite(ENABLE, VELOCIDAD_REVERSA);
}

void desactivarMotor(void) {
    pwmWrite(ENABLE, 0);
}

void secuenciaDispensado(void) {
    // Adelante por 1 segundo
    activarMotor();
    delay(1000);
    
    // Reversa breve
    motorReversa();
    delay(600);
    
    // Pausa breve
    desactivarMotor();
    delay(50);
    
    // Adelante de nuevo
    activarMotor();
    delay(500);
}
