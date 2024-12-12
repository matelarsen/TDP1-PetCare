#include "sapi.h"

typedef struct {
    uint8_t hora;
    uint8_t minuto;
    uint16_t gramos;
} Horario;

#define MAX_HORARIOS 10
Horario horarios[MAX_HORARIOS];
uint8_t cantidadHorarios = 0;

// Variables para simulación
uint16_t pesoSimuladoPlato = 100;      // Comienza con 100g
uint16_t pesoSimuladoDispensador = 1000; // Comienza con 1kg

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
    
    // Esperar indefinidamente por el header 'H'
    while (!horarioRecibido) {
        if (uartReadByte(UART_232, &c)) {
            if (c == 'H') {
                uartWriteString(UART_USB, "Header 'H' recibido\r\n");
                
                // Esperar cantidad de horarios
                while (!uartReadByte(UART_232, &cantidadHorarios));
                
                if (cantidadHorarios > MAX_HORARIOS || cantidadHorarios == 0) {
                    uartWriteString(UART_USB, "Error: cantidad de horarios inválida\r\n");
                    return false;
                }
                
                char debugMsg[50];
                sprintf(debugMsg, "Recibidos %d horarios:\r\n", cantidadHorarios);
                uartWriteString(UART_USB, debugMsg);
                
                // Recibir cada horario
                for (int i = 0; i < cantidadHorarios; i++) {
                    // Leer hora
                    while (!uartReadByte(UART_232, &horarios[i].hora));
                    
                    // Leer minuto
                    while (!uartReadByte(UART_232, &horarios[i].minuto));
                    
                    // Leer gramos (2 bytes)
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
        gpioToggle(LEDB); // Parpadear LED mientras espera
        delay(100);
    }
    
    return true;
}

void enviarPesos(void) {
    // Simular reducción de peso
    pesoSimuladoPlato = (pesoSimuladoPlato > 5) ? pesoSimuladoPlato - 5 : 0;
    pesoSimuladoDispensador = (pesoSimuladoDispensador > 2) ? 
                             pesoSimuladoDispensador - 2 : 0;
    
    uartWriteString(UART_USB, "Enviando pesos...\r\n");
    
    // Enviar datos
    uartWriteByte(UART_232, 'P');  // Header para pesos
    delay(10);
    
    // Enviar peso plato (2 bytes)
    uartWriteByte(UART_232, (uint8_t)(pesoSimuladoPlato >> 8));
    delay(10);
    uartWriteByte(UART_232, (uint8_t)(pesoSimuladoPlato & 0xFF));
    delay(10);
    
    // Enviar peso dispensador (2 bytes)
    uartWriteByte(UART_232, (uint8_t)(pesoSimuladoDispensador >> 8));
    delay(10);
    uartWriteByte(UART_232, (uint8_t)(pesoSimuladoDispensador & 0xFF));
    
    char debugMsg[100];
    sprintf(debugMsg, "Enviando pesos - Plato: %dg, Dispensador: %dg\r\n", 
            pesoSimuladoPlato, pesoSimuladoDispensador);
    uartWriteString(UART_USB, debugMsg);
}

int main(void) {
    boardInit();
    uartConfig(UART_232, 115200);
    uartConfig(UART_USB, 115200);
    
    gpioWrite(LEDB, OFF);
    
    while(1) {
        // Solicitar horarios y esperar hasta recibirlos
        solicitarHorarios();
        if (recibirYMostrarHorarios()) {
            gpioWrite(LEDB, ON);
            enviarPesos();
            gpioWrite(LEDB, OFF);
        }
        
        delay(30000); // Esperar 30 segundos antes del siguiente ciclo
    }
    
    return 0;
}

 