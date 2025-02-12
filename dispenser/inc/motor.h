/*=============================================================================
 * Author: mateo
 * Date: 2025/02/06
 * Version: 1.0
 *===========================================================================*/

#ifndef MOTOR_H
#define MOTOR_H

#include "sapi.h"

// Definiciones para el motor
#define DIR1 GPIO0
#define DIR2 GPIO1
#define ENABLE T_FIL2

void motorInit(void);
void activarMotor(void);
void desactivarMotor(void);

#endif