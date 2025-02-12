#include "rtc_control.h"

void rtcControlInit(void) {
    rtcInit();
}

void mostrarHoraActual(void) {
    rtc_t rtc;
    rtcRead(&rtc);
    char buffer[100];
    sprintf(buffer, "Hora actual: %02d:%02d:%02d\r\n", rtc.hour, rtc.min, rtc.sec);
    uartWriteString(UART_USB, buffer);
}
