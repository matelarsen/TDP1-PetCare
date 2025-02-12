#ifndef DISPENSER_H
#define DISPENSER_H

#include "sapi.h"
#include <math.h>
#include "motor.h"
#include "loadcell.h"
#include "esp_comm.h"
#include "rtc_control.h"

// Estructura para horarios
typedef struct {
    uint8_t hora;
    uint8_t minuto;
    uint16_t gramos;
} Horario;

#define MAX_HORARIOS 10
extern Horario horarios[MAX_HORARIOS];
extern uint8_t cantidadHorarios;

void servirComida(uint16_t gramosObjetivo);
void verificarHorarios(void);

#endif
