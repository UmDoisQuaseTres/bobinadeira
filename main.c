
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

#define _XTAL_FREQ  8000000UL  // Frequência do clock para as funções de delay

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
unsigned char rounds = 0x00;
unsigned char coreDiameter = 0x00;
unsigned char wire = 0x00;
__bit backLight = 0; // Controle do backlight (0 = apagado, 1 = ligado)

//=============================================================================
// === Prototipos das funcoes

void initialScreen(void);
void indutorScreen(void);
void trafoScreen(void);
void voltasScreen(void);
void bitolaScreen(void);

//=============================================================================
// === Funções para I2C

/**
 * @brief Envia um comando ao LCD via I2C
 * 
 * @param command O comando a ser enviado ao LCD
 */
void i2c_lcdCommand(uint8_t command) {
    uint8_t data;
    data = command & 0xF0;
    pcf8574Write(data | (backLight << BL) | (1 << EN));
    __delay_us(10);
    pcf8574Write(data | (backLight << BL));
    __delay_us(50);

    data = command << 4;
    pcf8574Write(data | (backLight << BL) | (1 << EN));
    __delay_us(10);
    pcf8574Write(data | (backLight << BL));
    __delay_us(50);
}

/**
 * @brief Envia dados ao LCD via I2C
 * 
 * @param command O dado a ser enviado ao LCD
 */
void i2c_lcdData(uint8_t command) {
    uint8_t data;
    data = command & 0xF0;
    pcf8574Write(data | (backLight << BL) | (1 << EN) | (1 << RS));
    __delay_us(10);
    pcf8574Write(data | (backLight << BL) | (1 << RS));
    __delay_us(50);

    data = command << 4;
    pcf8574Write(data | (backLight << BL) | (1 << EN) | (1 << RS));
    __delay_us(10);
    pcf8574Write(data | (backLight << BL) | (1 << RS));
    __delay_us(50);
}

/**
 * @brief Define a posição do cursor no LCD
 * 
 * @param x Coluna (1 a 20)
 * @param y Linha (1 a 4)
 */
void i2c_lcdXY(int8_t x, int8_t y) {
    int8_t addr[] = {0x80, 0xC0, 0x94, 0xD4}; // Endereços de início de linha para LCD 20x4
    if (y >= 1 && y <= 4 && x >= 1 && x <= 20) {
        i2c_lcdCommand(addr[y - 1] + (x - 1));
    }
}

/**
 * @brief Envia uma string de texto para o LCD
 * 
 * @param txt Ponteiro para a string a ser exibida
 */
void i2c_lcdText(const char *txt) {
    while (*txt) {
        i2c_lcdData(*txt++);
    }
}
/**
 * @brief Limpa o display LCD
 */
void i2c_lcdClear(void) {
    i2c_lcdCommand(0x01); // Comando para limpar o LCD
    __delay_ms(5); // Aguardar o LCD processar o comando
}

/**
 * @brief Inicializa o LCD em modo 4-bits utilizando o PCF8574A via I2C
 */
void i2c_lcdInit(void) {
    i2c_init(100000); // Inicializa o I2C com baud rate de 100kHz
    __delay_us(10);
    pcf8574Write(0);
    __delay_ms(10);
    i2c_lcdCommand(0x33); // Inicialização padrão do LCD
    __delay_us(10);
    i2c_lcdCommand(0x32);
    __delay_us(10);
    i2c_lcdCommand(0x28); // Modo 4-bits, 2 linhas
    __delay_us(10);
    i2c_lcdCommand(0x0F); // Liga o display, cursor ligado e piscando
    __delay_us(10);
    i2c_lcdCommand(0x01); // Limpa o display
    __delay_ms(5);
    i2c_lcdCommand(0x06); // Incrementa automaticamente o cursor
    __delay_us(10);
}

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
    i2c_lcdXY(2, 1);
    i2c_lcdText("Bobinadeira 0.0.1");
    i2c_lcdXY(3, 3);
    i2c_lcdText(" Init system....");
    __delay_ms(3000);
    i2c_lcdClear(); // Limpa o LCD


    //=============================================================================
    // === Limpando os caracteres

    //Loop infinito
    while (1) {

        //=============================================================================
        // === Chamando as funcoes

        initialScreen();
        indutorScreen();
        trafoScreen();
        //initI2C(); // Inicializar I2C


    }
    return 0;
}

//=============================================================================
// === Tela inicial

void initialScreen() {
    flag = 0x01;
    i2c_lcdXY(1, 1);
    i2c_lcdText("Selecione o produto:");
    i2c_lcdXY(1, 4);
    i2c_lcdText("(A)Indutor");
    i2c_lcdXY(13, 4);
    i2c_lcdText("(B)Trafo");
}

//=============================================================================
// === Tela do indutor

void indutorScreen() {
    if (SA == 0x00) {
        // Limpa o LCD
        i2c_lcdClear();
        __delay_ms(100);

        // Enquanto a flag for 1...
        while (flag == 0x01) {
            i2c_lcdXY(1, 1);
            i2c_lcdText("Diametro do nucleo:");
            i2c_lcdXY(14, 4);
            i2c_lcdText("(b)Prox");
            i2c_lcdXY(1, 4);
            i2c_lcdText("(c)Canc");
            __delay_ms(100);

            // Se SD for pressionado incrementa o tamanho do nucleo
            if (SD == 0x00) {
                coreDiameter++;
                __delay_ms(100);
            }
            if (SA == 0x00 && coreDiameter > 0) {
                coreDiameter--;
                __delay_ms(100);
            }

      /*      //=============================================================================
            // === Calcula o numero de digitos

            unsigned char numDigits = 0;
            unsigned char tempCoreDiameter = coreDiameter;
            if (tempCoreDiameter == 0) {
                numDigits = 1;
            } else {
                while (tempCoreDiameter > 0) {
                    numDigits++;
                    tempCoreDiameter /= 10;
                }
            }

            //=============================================================================
            // === Calcula o tamanho do texto a ser exibido

            unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
            unsigned char lcdWidth = 20; // Número de colunas do LCD
            unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

            //=============================================================================
            // === Ajusta a posição do cursor automaticamente

            i2c_lcdXY(3, startPos);
            i2c_lcdText(coreDiameter);
            i2c_lcdXY(3, startPos + numDigits);
            i2c_lcdText(" mm");
            __delay_ms(50);
*/
            // Se o botão próximo for pressionado...
            if (SB == 0x00) {
                __delay_ms(100);
                i2c_lcdClear();
                voltasScreen();
            }

            // Clicando em cancelar limpa o LCD, o bit do diâmetro e retorna à tela principal
            if (SC == 0x00) {
                i2c_lcdClear();
                coreDiameter = 0x00;
                flag = 0x00;
            }
        }
    }
}


//=============================================================================
// === Tela do trafo

void trafoScreen() {
    if (SB == 0x00) {
        __delay_ms(100);
        i2c_lcdClear();
        while (flag == 0x01) {
            i2c_lcdXY(1, 1);
            i2c_lcdText("Diametro do nucleo:");
            i2c_lcdXY(14, 4);
            i2c_lcdText("(b)Prox");
            i2c_lcdXY(1, 4);
            i2c_lcdText("(c)Canc");
            __delay_ms(100);

            if (SD == 0x00) {
                coreDiameter++;
                __delay_ms(100);
            }
            if (SA == 0x00 && coreDiameter > 0) {
                coreDiameter--;
                __delay_ms(100);
            }

            //=============================================================================
            // === Calcula o numero de digitos

            unsigned char numDigits = 0;
            unsigned char tempCoreDiameter = coreDiameter;
            if (tempCoreDiameter == 0) {
                numDigits = 1;
            } else {
                while (tempCoreDiameter > 0) {
                    numDigits++;
                    tempCoreDiameter /= 10;
                }
            }

            //=============================================================================
            // === Calcula o tamanho do texto a ser exibido

            unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
            unsigned char lcdWidth = 20; // Número de colunas do LCD
            unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

            //=============================================================================
            // === Ajusta a posição do cursor automaticamente

            i2c_lcdXY(3, startPos);
            i2c_lcdText(coreDiameter);
            i2c_lcdXY(3, startPos + numDigits);
            i2c_lcdText(" mm");
            __delay_ms(50);

            if (SB == 0x00) {
                __delay_ms(100);
                i2c_lcdClear();
                voltasScreen();
            }
            if (SC == 0x00) {
                i2c_lcdClear();
                coreDiameter = 0x00;
                flag = 0x00;
            }
        }
    }
}

//=============================================================================
// === Tela de voltas

void voltasScreen() {
    flag = 0x02;
    i2c_lcdClear();
    while (flag == 0x02) {
        i2c_lcdXY(1, 1);
        i2c_lcdText("Quantas voltas:");
        i2c_lcdXY(14, 4);
        i2c_lcdText("(b)Prox");
        i2c_lcdXY(1, 4);
        i2c_lcdText("(c)Canc");
        __delay_ms(100);

        if (SD == 0x00) {
            rounds++;
            __delay_ms(100);
        }
        if (SA == 0x00 && rounds > 0) {
            rounds--;
            __delay_ms(100);
        }

        //=============================================================================
        // === Calcula o numero de digitos

        unsigned char numDigits = 0;
        unsigned char tempRounds = rounds;
        if (tempRounds == 0) {
            numDigits = 1;
        } else {
            while (tempRounds > 0) {
                numDigits++;
                tempRounds /= 10;
            }
        }

        //=============================================================================
        // === Calcula o tamanho do texto a ser exibido

        unsigned char textLength = numDigits + 6; // Número de dígitos + " Voltas"
        unsigned char lcdWidth = 20; // Número de colunas do LCD
        unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

        //=============================================================================
        // === Ajusta a posição do cursor automaticamente

        i2c_lcdXY(3, startPos);
        i2c_lcdText(rounds);
        i2c_lcdXY(3, startPos + numDigits);
        i2c_lcdText(" Voltas");
        __delay_ms(50);

        if (SB == 0x00) {
            __delay_ms(100);
            i2c_lcdClear();
            bitolaScreen();
        }
        if (SC == 0x00) {
            i2c_lcdClear();
            rounds = 0x00;
            flag = 0x00;
        }
    }
}

//=============================================================================
// === Tela de bitola AWG

void bitolaScreen() {
    flag = 0x02;
    i2c_lcdClear();
    while (flag == 0x02) {
        i2c_lcdXY(1, 1);
        i2c_lcdText("Qual a bitola AWG?:");
        i2c_lcdXY(14, 4);
        i2c_lcdText("(b)Init");
        i2c_lcdXY(1, 4);
        i2c_lcdText("(c)Canc");
        __delay_ms(100);

        // Se SD for pressionado, incrementa a bitola
        if (SD == 0x00) {
            wire++;
            __delay_ms(200);
        }
        if (SA == 0x00 && wire > 0) {
            wire--;
            __delay_ms(200);
        }

        //=============================================================================
        // === Calcula o numero de digitos

        unsigned char numDigits = 0;
        unsigned char tempWire = wire;
        if (tempWire == 0) {
            numDigits = 1;
        } else {
            while (tempWire > 0) {
                numDigits++;
                tempWire /= 10;
            }
        }

        //=============================================================================
        // === Calcula o tamanho do texto a ser exibido

        unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
        unsigned char lcdWidth = 20; // Número de colunas do LCD
        unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

        //=============================================================================
        // === Ajusta a posição do cursor automaticamente

        i2c_lcdXY(3, startPos);
        i2c_lcdText(wire);
        i2c_lcdXY(3, startPos + numDigits);
        i2c_lcdText(" mm");
        __delay_ms(50);

        // Clicando em próximo
        if (SB == 0x00) {
            i2c_lcdClear();
            // Chama a próxima tela aqui
            // Exemplo: bitolaScreen(); (se necessário)
        }

        // Clicando em cancelar reseta o bit de controle e retorna à tela principal
        if (SC == 0x00) {
            i2c_lcdClear();
            wire = 0x00;
            flag = 0x00;
        }
    }
}

//=============================================================================
// === Configuração do I2C - Futuro

