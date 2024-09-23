/*
 * File:   timer0init.c
 * Author: deoli
 *
 * Created on 23 de Setembro de 2024, 00:25
 */


#include <xc.h>
#include "delaytmr.h"

void Timer0_Init() {
    T0CON = 0b00000000;  // Habilita Timer0, modo de 16 bits, sem prescaler
    TMR0H = 0xFF;        // Preload para 1 ms (65536 - 250 = 65286, que é 0xFF06)
    TMR0L = 0x06;
    TMR0IF = 0;          // Limpa a flag de interrupção
    TMR0ON = 1;          // Liga o Timer0
}
void delay_ms(unsigned int ms) {
    while(ms > 0) {
        Timer0_Init();    // Inicializa o Timer0
        while(!TMR0IF);   // Espera até que a flag TMR0IF seja setada
        TMR0ON = 0;       // Desliga o Timer0
        TMR0IF = 0;       // Limpa a flag de interrupção
        ms--;             // Decrementa o contador de milissegundos
    }
}