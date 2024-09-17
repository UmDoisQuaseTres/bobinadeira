// config.h
// Configura��es e defini��es do projeto bobinadeira autom�tica para trafos e indutores

#ifndef CONFIG_H
#define CONFIG_H

//=============================================================================
// === PIC16F877A Configuration Bit Settings ===

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//=============================================================================
// === Clock do MCU

#define _XTAL_FREQ  8000000UL  // Frequ�ncia do clock para as fun��es de delay

// Defini��es dos pinos no PCF8574A para controlar o LCD
#define RS  0  // Pino RS do LCD (Registrador de dados ou comando)
#define RW  1  // Pino RW do LCD (Leitura/Escrita)
#define EN  2  // Pino Enable do LCD
#define BL  3  // Pino Backlight do LCD (opcional)

//=============================================================================
// === Botoes de interacao
#define SA RB1  // Bot�o para selecionar 'Indutor'
#define SB RB2  // Bot�o para selecionar 'Trafo'
#define SC RB3  // Bot�o para voltar � tela inicial
#define SE RB4  // Bot�o para incrementar valores
#define SF RB5  // Bot�o para decrementar valores

// Outras poss�veis defini��es ou macros
#define LCD_BACKLIGHT_ON  1
#define LCD_BACKLIGHT_OFF 0

#endif // CONFIG_H
