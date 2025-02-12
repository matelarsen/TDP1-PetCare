#include "dispenser.h"

Horario horarios[MAX_HORARIOS];
uint8_t cantidadHorarios = 0;

void servirComida(uint16_t gramosObjetivo) {
    float pesoInicial = ObtenerPesoPlato();
    float pesoActual;
    char debugMsg[50];
    
    uartWriteString(UART_USB, "Iniciando dispensado de comida...\r\n");
    sprintf(debugMsg, "Peso inicial: %.2fg\r\n", pesoInicial);
    uartWriteString(UART_USB, debugMsg);
    
    activarMotor();
    
    do {
        pesoActual = ObtenerPesoPlato();
        float diferencia = pesoActual - pesoInicial;
        
        sprintf(debugMsg, "Peso actual: %.2fg - Objetivo: %dg - Diferencia: %.2fg\r\n", 
                pesoActual, gramosObjetivo, diferencia);
        uartWriteString(UART_USB, debugMsg);
        
        if (diferencia >= gramosObjetivo || 
            fabs(diferencia - gramosObjetivo) <= (gramosObjetivo * 0.05)) {
            desactivarMotor();
            sprintf(debugMsg, "Objetivo alcanzado. Peso final: %.2fg\r\n", pesoActual);
            uartWriteString(UART_USB, debugMsg);
            break;
        }
        
        delay(100);
    } while(1);
    
    float pesoDispensador = ObtenerPesoDispensador();
    enviarPesos(pesoActual, pesoDispensador);
    
    uartWriteString(UART_USB, "Esperando 1 minuto antes de siguiente verificación...\r\n");
    delay(60000);
}

void verificarHorarios() {
    rtc_t rtc;
    rtcRead(&rtc);
    
    for(int i = 0; i < cantidadHorarios; i++) {
        if(rtc.hour == horarios[i].hora && rtc.min == horarios[i].minuto) {
            servirComida(horarios[i].gramos);
            break;
        }
    }
}

int main(void) {
    boardInit();
    
    // Inicializaciones de módulos
    motorInit();
    loadcellInit();
    espCommInit();
    rtcControlInit();
    
    gpioWrite(LEDB, OFF);
    mostrarHoraActual();
    
    while(1) {
        solicitarHorarios();
        if (recibirYMostrarHorarios()) {
            gpioWrite(LEDB, ON);
            float pesoPlato = ObtenerPesoPlato();
            float pesoDispensador = ObtenerPesoDispensador() * 2.65;
            enviarPesos(pesoPlato, pesoDispensador);
            gpioWrite(LEDB, OFF);
        }
        mostrarHoraActual();
        verificarHorarios();
        delay(1000);
    }
    
    return 0;
}