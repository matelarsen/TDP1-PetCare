#ifndef ESP_COMM_H
#define ESP_COMM_H

#include "sapi.h"
#include <stdbool.h>

void espCommInit(void);
void limpiarBuffer(void);
void solicitarHorarios(void);
bool recibirYMostrarHorarios(void);
void enviarPesos(float pesoPlato, float pesoDispensador);

#endif
