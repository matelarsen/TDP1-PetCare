#include "esp_comm.h"
#include "dispenser.h"

void espCommInit(void) {
    uartConfig(UART_232, 115200);
    uartConfig(UART_USB, 115200);
}

void limpiarBuffer(void) {
    char c;
    while(uartReadByte(UART_232, &c));
}

void solicitarHorarios(void) {
    uartWriteString(UART_USB, "Solicitando horarios...\r\n");
    uartWriteByte(UART_232, 'R');
}

bool recibirYMostrarHorarios(void) {
    char c;
    bool horarioRecibido = false;
    
    limpiarBuffer();
    uartWriteString(UART_USB, "Esperando horarios...\r\n");
    
    while (!horarioRecibido) {
        if (uartReadByte(UART_232, &c)) {
            if (c == 'H') {
                uartWriteString(UART_USB, "Header 'H' recibido\r\n");
                
                while (!uartReadByte(UART_232, &cantidadHorarios));
                
                if (cantidadHorarios > MAX_HORARIOS || cantidadHorarios == 0) {
                    uartWriteString(UART_USB, "Error: cantidad de horarios inválida\r\n");
                    return false;
                }
                
                char debugMsg[50];
                sprintf(debugMsg, "Recibidos %d horarios:\r\n", cantidadHorarios);
                uartWriteString(UART_USB, debugMsg);
                
                for (int i = 0; i < cantidadHorarios; i++) {
                    while (!uartReadByte(UART_232, &horarios[i].hora));
                    while (!uartReadByte(UART_232, &horarios[i].minuto));
                    
                    uint8_t gramosHigh, gramosLow;
                    while (!uartReadByte(UART_232, &gramosHigh));
                    while (!uartReadByte(UART_232, &gramosLow));
                    
                    horarios[i].gramos = (gramosHigh << 8) | gramosLow;
                    
                    sprintf(debugMsg, "Horario %d: %02d:%02d - %dg\r\n", 
                            i+1, horarios[i].hora, horarios[i].minuto, horarios[i].gramos);
                    uartWriteString(UART_USB, debugMsg);
                }
                
                horarioRecibido = true;
                uartWriteString(UART_USB, "Todos los horarios recibidos correctamente\r\n");
            }
        }
        gpioToggle(LEDB);
        delay(100);
    }
    
    return true;
}

void enviarPesos(float pesoPlato, float pesoDispensador) {
    uint16_t pesoP = (uint16_t)pesoPlato;
    uint16_t pesoD = (uint16_t)pesoDispensador;
    
    uartWriteString(UART_USB, "Enviando pesos...\r\n");
    
    uartWriteByte(UART_232, 'P');
    delay(10);
    
    uartWriteByte(UART_232, (uint8_t)(pesoP >> 8));
    delay(10);
    uartWriteByte(UART_232, (uint8_t)(pesoP & 0xFF));
    delay(10);
    
    uartWriteByte(UART_232, (uint8_t)(pesoD >> 8));
    delay(10);
    uartWriteByte(UART_232, (uint8_t)(pesoD & 0xFF));
    
    char debugMsg[100];
    sprintf(debugMsg, "Enviando pesos - Plato: %dg, Dispensador: %dg\r\n", 
            pesoP, pesoD);
    uartWriteString(UART_USB, debugMsg);
}
