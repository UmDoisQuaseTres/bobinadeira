
//Projeto bobinadeira automática para trafos e indutores
//Autor: Matheus de Oliveira
//07-09-2024

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

#define _XTAL_FREQ  1000000UL  // Frequência do clock para as funções de delay

//=============================================================================
// === Botoes de interacao

#define SA RB1
#define SB RB2
#define SC RB3
#define SD RB4

//=============================================================================
// === Saidas para o LCD

// Definições dos pinos no PCF8574A para controlar o LCD
#define RS  0  // Pino RS do LCD (Registrador de dados ou comando)
#define RW  1  // Pino RW do LCD (Leitura/Escrita)
#define EN  2  // Pino Enable do LCD
#define BL  3  // Pino Backlight do LCD (opcional)

//=============================================================================
// === Bibliotecas

#include <xc.h>
#include "pcf8574.h"  // Inclua a biblioteca PCF8574
#include <stdio.h>

//=============================================================================
// === Variaveis globais

unsigned char flag = 0x00;
unsigned char flag_more = 0x00;
unsigned char flag_minus = 0x00;
unsigned short screen = 0x00;
unsigned short previousScreen = 0x00;
unsigned char nTela = 3;
__bit backLight = 0; // Controle do backlight (0 = apagado, 1 = ligado)

//=============================================================================
// === Prototipos das funcoes
void i2c_lcdCommand(uint8_t command);
void i2c_lcdData(uint8_t command);
void i2c_lcdXY(int8_t x, int8_t y);
void i2c_lcdText(const char *txt);
void i2c_lcdClear(void);
void i2c_lcdInit(void);
void read_buts(void);
void initialScreen(void);

//=============================================================================

int main() {
    //=============================================================================
    // === Pinos RC3 e RC4 como entrada I2C

    TRISC |= 0x18;

    //=============================================================================
    // === Pinos RD2 a RD7 como saída digital

    TRISD &= 0x03;

    //=============================================================================
    // === Pinos RB1 a RB5 como enrada digital

    TRISB |= 0x3E;

    //=============================================================================
    // === Inicializando o sistema

    i2c_lcdInit(); // Inicializa o LCD via I2C
    backLight = 1; // Liga o backlight do LCD
    __delay_ms(1000); // Atraso de 1 segundo

    // Exibe mensagens iniciais no LCD
    i2c_lcdXY(2, 1); //Posicionando cursor
    i2c_lcdText("Bobinadeira 0.0.1");
    i2c_lcdXY(3, 3); //Posicionando cursor
    i2c_lcdText(" Init system....");
    __delay_ms(3000); //Aguardo...
    i2c_lcdClear(); // Limpa o LCD

    //=============================================================================
    // === Limpando os caracteres

    // Loop principal
    while (1) {
        read_buts(); // Chama função para leitura dos botões

        // Só atualiza a tela se o valor de screen mudou
        if (screen != previousScreen) {
            i2c_lcdClear(); // Limpa a tela
            switch (screen) {
                case 0x01: 
                    initialScreen(); // Mostra a tela inicial
                    break;
                // Adicione os outros menus aqui, como menu2(), menu3(), etc.
            }
            previousScreen = screen; // Atualiza o estado anterior
        }
    }
    return 0;
}

// === Funções para I2C

// Funções auxiliares para escrever um comando ou dado no LCD

void i2c_lcdCommand(uint8_t command) {
    uint8_t upper_nibble = command & 0xF0;
    uint8_t lower_nibble = (command << 4) & 0xF0;

    // Enviar parte alta dos comandos
    pcf8574Write(upper_nibble | (backLight << BL) | (1 << EN));
    __delay_us(10);
    pcf8574Write(upper_nibble | (backLight << BL));
    __delay_us(50);

    // Enviar parte baixa dos comandos
    pcf8574Write(lower_nibble | (backLight << BL) | (1 << EN));
    __delay_us(10);
    pcf8574Write(lower_nibble | (backLight << BL));
    __delay_us(50);
}

void i2c_lcdData(uint8_t data) {
    uint8_t upper_nibble = data & 0xF0;
    uint8_t lower_nibble = (data << 4) & 0xF0;

    // Enviar parte alta dos dados
    pcf8574Write(upper_nibble | (backLight << BL) | (1 << EN) | (1 << RS));
    __delay_us(10);
    pcf8574Write(upper_nibble | (backLight << BL) | (1 << RS));
    __delay_us(50);

    // Enviar parte baixa dos dados
    pcf8574Write(lower_nibble | (backLight << BL) | (1 << EN) | (1 << RS));
    __delay_us(10);
    pcf8574Write(lower_nibble | (backLight << BL) | (1 << RS));
    __delay_us(50);
}

// Função para posicionar o cursor

void i2c_lcdXY(int8_t x, int8_t y) {
    int8_t addr[] = {0x80, 0xC0, 0x94, 0xD4}; // Endereços de início de linha para LCD 20x4
    if (y >= 1 && y <= 4 && x >= 1 && x <= 20) {
        i2c_lcdCommand(addr[y - 1] + (x - 1));
    }
}

// Função para escrever um texto completo no LCD de uma vez só

void i2c_lcdText(const char *txt) {
    while (*txt) {
        i2c_lcdData(*txt++);
    }
}

// Função para limpar o LCD

void i2c_lcdClear(void) {
    i2c_lcdCommand(0x01); // Comando para limpar o LCD
    __delay_ms(10); // Aguardar o LCD processar o comando
}

// Inicializa o LCD em modo 4-bits utilizando o PCF8574A via I2C

void i2c_lcdInit(void) {
    i2c_init(100000); // Inicializa o I2C com baud rate de 100kHz
    __delay_us(10);
    pcf8574Write(0);
    __delay_ms(10);
    i2c_lcdCommand(0x33); // Inicialização padrão do LCD
    __delay_us(10);
    i2c_lcdCommand(0x32); // Configura o LCD para 4 bits
    __delay_us(10);
    i2c_lcdCommand(0x28); // Modo 4-bits, 2 linhas
    __delay_us(10);
    i2c_lcdCommand(0x0F); // Liga o display e desliga o cursor
    __delay_us(10);
    i2c_lcdCommand(0x01); // Limpa o display
    __delay_ms(10); // Aguardar o LCD processar o comando
    i2c_lcdCommand(0x06); // Incrementa automaticamente o cursor
    __delay_us(10);
}
//=============================================================================
// === Tela inicial

void initialScreen() {
    i2c_lcdText("Selecione o produto:");
    i2c_lcdXY(1, 4);
    i2c_lcdText("(A)Indutor");
    i2c_lcdXY(13, 4);
    i2c_lcdText("(B)Trafo");

}

void read_buts() {
    // Verifica se o botão "menos" foi pressionado
    if (SA) {
        flag_minus = 0x01; // Seta a flag quando o botão "menos" for pressionado
    }
    // Verifica se o botão "mais" foi pressionado
    if (SB) {
        flag_more = 0x01; // Seta a flag quando o botão "mais" for pressionado
    }

    // Se o botão "menos" foi solto e a flag estava setada
    if (SA && flag_minus) {
        flag_minus = 0x00; // Limpa a flag
        screen--; // Decrementa a tela
        if (screen < 0x01) {
            screen = nTela; // Se o valor de screen for menor que 1, retorna ao valor máximo
        }
        i2c_lcdClear(); // Limpa a tela, substituindo o lcd_clr
    }

    // Se o botão "mais" foi solto e a flag estava setada
    if (SB && flag_more) {
        flag_more = 0x00; // Limpa a flag
        screen++; // Incrementa a tela
        if (screen > nTela) {
            screen = 0x01; // Se o valor de screen for maior que o valor máximo, retorna a 1
        }
        i2c_lcdClear(); // Limpa a tela, substituindo o lcd_clr
    }
}
