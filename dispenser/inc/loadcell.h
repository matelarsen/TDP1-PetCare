#ifndef LOADCELL_H
#define LOADCELL_H

#include "sapi.h"
#include <math.h>  // Agregado para fabs()

#define HX711_SCK_PIN   GPIO3
#define HX711_DATA_PIN1 GPIO4  // Celda dispensador
#define HX711_DATA_PIN2 GPIO5  // Celda plato

void loadcellInit(void);
void Ready4read(int pin);
float ObtenerPesoDispensador(void);
float ObtenerPesoPlato(void);
void Tarar(int pin, int32_t* tara_val);

#endif