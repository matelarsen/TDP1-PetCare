// motor.h
#ifndef MOTOR_H
#define MOTOR_H

#include "sapi.h"

// Definiciones para el motor
#define DIR1 GPIO0
#define DIR2 GPIO1
#define ENABLE T_FIL2
#define VELOCIDAD_NORMAL 128  // 50% de velocidad
#define VELOCIDAD_REVERSA 100 // ~40% para reversa

void motorInit(void);
void activarMotor(void);
void desactivarMotor(void);
void motorReversa(void);
void secuenciaDispensado(void);

#endif
