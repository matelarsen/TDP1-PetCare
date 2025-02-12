#include "loadcell.h"

static int32_t tara1 = 0;  // Tara dispensador
static int32_t tara2 = 0;  // Tara plato
const float escala = 0.00064617f;

void loadcellInit(void) {
    Ready4read(HX711_DATA_PIN1);
    Ready4read(HX711_DATA_PIN2);
    Tarar(HX711_DATA_PIN1, &tara1);
    Tarar(HX711_DATA_PIN2, &tara2);
}

void Ready4read(int pin) {
    gpioConfig(pin, GPIO_INPUT);
    gpioConfig(HX711_SCK_PIN, GPIO_OUTPUT);
    while (gpioRead(pin));
}

int32_t ReadRaw(int pin) {
    int32_t Count = 0;
    uint32_t i;

    gpioWrite(HX711_SCK_PIN, LOW);
    while (gpioRead(pin));

    for (i = 0; i < 24; i++) {
        gpioWrite(HX711_SCK_PIN, HIGH);
        Count = Count << 1;
        gpioWrite(HX711_SCK_PIN, LOW);
        if (gpioRead(pin)) {
            Count++;
        }
    }

    gpioWrite(HX711_SCK_PIN, HIGH);
    gpioWrite(HX711_SCK_PIN, LOW);

    if (Count & 0x800000) {
        Count |= 0xFF000000;
    }

    return Count;
}

void Tarar(int pin, int32_t* tara_val) {
    *tara_val = 0;
    int32_t acum = 0;
    for (uint32_t i = 0; i < 10; i++) {
        acum += ReadRaw(pin);
        delay(50);
    }
    *tara_val = acum / 10;
    
    char msg[50];
    sprintf(msg, "Valor de tara calculado: %d\n", *tara_val);
    uartWriteString(UART_USB, msg);
}

float ObtenerPeso(int pin, int32_t tara_val) {
    int32_t acum = 0;
    for (int i = 0; i < 5; i++) {
        acum += ReadRaw(pin);
        delay(10);
    }
    int32_t rawValue = acum / 5;
    float peso = (float)(rawValue - tara_val) * escala;
    return fabs(peso);
}

float ObtenerPesoDispensador(void) {
    return ObtenerPeso(HX711_DATA_PIN1, tara1);
}

float ObtenerPesoPlato(void) {
    return ObtenerPeso(HX711_DATA_PIN2, tara2);
}
