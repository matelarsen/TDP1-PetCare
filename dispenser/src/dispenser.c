#include "dispenser.h"

Horario horarios[MAX_HORARIOS];
uint8_t cantidadHorarios = 0;
#define TIEMPO_ENTRE_SOLICITUDES 10  // 10 segundos

void servirComida(uint16_t gramosObjetivo) {
    float pesoInicial = ObtenerPesoPlato();
    float pesoActual;
    char debugMsg[50];
    
    uartWriteString(UART_USB, "Iniciando dispensado de comida...\r\n");
    sprintf(debugMsg, "Peso inicial: %.2fg\r\n", pesoInicial);
    uartWriteString(UART_USB, debugMsg);
    
    while(1) {
        secuenciaDispensado();
        
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
    }
    
    float pesoDispensador = ObtenerPesoDispensador();
    enviarPesos(pesoActual, pesoDispensador);
    
    // Limpiar buffer UART después de dispensar
    limpiarBuffer();
    
    uartWriteString(UART_USB, "Esperando 1 minuto antes de siguiente verificación...\r\n");
    delay(60000);
}

void verificarHorarios() {
    rtc_t rtc;
    rtcRead(&rtc);
    
    for(int i = 0; i < cantidadHorarios; i++) {
        if(rtc.hour == horarios[i].hora && rtc.min == horarios[i].minuto) {
            servirComida(horarios[i].gramos);
            return;  // Salir después de dispensar
        }
    }
}

int main(void) {
    boardInit();
    
    motorInit();
    loadcellInit();
    espCommInit();
    rtcControlInit();
    
    gpioWrite(LEDB, OFF);
    mostrarHoraActual();
    
    uint8_t contadorSegundos = 0;
    
    while(1) {
        contadorSegundos++;
        
        if (contadorSegundos >= TIEMPO_ENTRE_SOLICITUDES) {
            // Limpiar buffer antes de solicitar
            limpiarBuffer();
            
            mostrarHoraActual();
            solicitarHorarios();
            
            if (recibirYMostrarHorarios()) {
                gpioWrite(LEDB, ON);
                float pesoPlato = ObtenerPesoPlato();
                float pesoDispensador = ObtenerPesoDispensador() * 2.65;
                enviarPesos(pesoPlato, pesoDispensador);
                gpioWrite(LEDB, OFF);
            }
            
            contadorSegundos = 0;
        }
        
        verificarHorarios();
        delay(1000);
    }
    
    return 0;
}
