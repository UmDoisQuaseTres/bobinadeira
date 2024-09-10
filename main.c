
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

#define _XTAL_FREQ 8000000

//=============================================================================
// === Botoes de interacao

#define SA RB1
#define SB RB2
#define SC RB3
#define SD RB4

//=============================================================================
// === Saidas para o LCD

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//=============================================================================
// === Bibliotecas

#include <xc.h>
#include "lcd.h"

//=============================================================================
// === Variaveis globais

unsigned char flag = 0x00;
unsigned char rounds = 0x00;
unsigned char coreDiameter = 0x00;
unsigned char wire = 0x00;

//=============================================================================
// === Prototipos das funcoes

//Toda a parte do protocolo I2C
void initI2C(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(unsigned char data);
unsigned char I2C_Read(unsigned char ack);

//Restante das funções base
void initialScreen(void);
void indutorScreen(void);
void trafoScreen(void);
void voltasScreen(void);
void bitolaScreen(void);

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
    Lcd_Init();
    Lcd_Set_Cursor(1, 2);
    Lcd_Write_String("Bobinadeira 0.0.1");
    Lcd_Set_Cursor(3, 5);
    Lcd_Write_String("Init system");
    for (int i = 0; i < 4; i++) {
        Lcd_Write_Char('.'); // Adiciona um ponto
        __delay_ms(500); // Espera 500ms (meio segundo) entre cada ponto
    }

    //=============================================================================
    // === Limpando os caracteres

    Lcd_Clear();
    __delay_ms(50);

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
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Selecione o produto:");
    Lcd_Set_Cursor(4, 1);
    Lcd_Write_String("(A)Indutor");
    Lcd_Set_Cursor(4, 13);
    Lcd_Write_String("(B)Trafo");
}

//=============================================================================
// === Tela do indutor

void indutorScreen() {
    if (SA == 0x00) {
        // Limpa o LCD
        Lcd_Clear();

        // Enquanto a flag for 1...
        while (flag == 0x01) {
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("Diametro do nucleo:");
            Lcd_Set_Cursor(4, 14);
            Lcd_Write_String("(b)Prox");
            Lcd_Set_Cursor(4, 1);
            Lcd_Write_String("(c)Canc");
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

            Lcd_Set_Cursor(3, startPos);
            Lcd_Write_Number(coreDiameter);
            Lcd_Set_Cursor(3, startPos + numDigits);
            Lcd_Write_String(" mm");
            __delay_ms(50);

            // Se o botão próximo for pressionado...
            if (SB == 0x00) {
                __delay_ms(100);
                Lcd_Clear();
                voltasScreen();
            }

            // Clicando em cancelar limpa o LCD, o bit do diâmetro e retorna à tela principal
            if (SC == 0x00) {
                Lcd_Clear();
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
        Lcd_Clear();
        while (flag == 0x01) {
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("Diametro do nucleo:");
            Lcd_Set_Cursor(4, 14);
            Lcd_Write_String("(b)Prox");
            Lcd_Set_Cursor(4, 1);
            Lcd_Write_String("(c)Canc");
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

            Lcd_Set_Cursor(3, startPos);
            Lcd_Write_Number(coreDiameter);
            Lcd_Set_Cursor(3, startPos + numDigits);
            Lcd_Write_String(" mm");
            __delay_ms(50);

            if (SB == 0x00) {
                __delay_ms(100);
                Lcd_Clear();
                voltasScreen();
            }
            if (SC == 0x00) {
                Lcd_Clear();
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
    Lcd_Clear();
    while (flag == 0x02) {
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("Quantas voltas:");
        Lcd_Set_Cursor(4, 14);
        Lcd_Write_String("(b)Prox");
        Lcd_Set_Cursor(4, 1);
        Lcd_Write_String("(c)Canc");
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

        Lcd_Set_Cursor(3, startPos);
        Lcd_Write_Number(rounds);
        Lcd_Set_Cursor(3, startPos + numDigits);
        Lcd_Write_String(" Voltas");
        __delay_ms(50);

        if (SB == 0x00) {
            __delay_ms(100);
            Lcd_Clear();
            bitolaScreen();
        }
        if (SC == 0x00) {
            Lcd_Clear();
            rounds = 0x00;
            flag = 0x00;
        }
    }
}

//=============================================================================
// === Tela de bitola AWG

void bitolaScreen() {
    flag = 0x02;
    Lcd_Clear();
    while (flag == 0x02) {
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("Qual a bitola AWG?:");
        Lcd_Set_Cursor(4, 14);
        Lcd_Write_String("(b)Init");
        Lcd_Set_Cursor(4, 1);
        Lcd_Write_String("(c)Canc");
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

        Lcd_Set_Cursor(3, startPos);
        Lcd_Write_Number(wire);
        Lcd_Set_Cursor(3, startPos + numDigits);
        Lcd_Write_String(" mm");
        __delay_ms(50);

        // Clicando em próximo
        if (SB == 0x00) {
            Lcd_Clear();
            // Chama a próxima tela aqui
            // Exemplo: bitolaScreen(); (se necessário)
        }

        // Clicando em cancelar reseta o bit de controle e retorna à tela principal
        if (SC == 0x00) {
            Lcd_Clear();
            wire = 0x00;
            flag = 0x00;
        }
    }
}

//=============================================================================
// === Configuração do I2C - Futuro

void initI2C() {
    // Configura o PIC para I2C Master
    SSPCON = 0b00101000; // I2C Master mode, clock = Fosc / (4 * (SSPADD+1))
    SSPCON2 = 0; // Desativa funções especiais extras
    SSPADD = 19; // Define o clock para I2C de 100 kHz (com Fosc = 8 MHz)
    SSPSTAT = 0; // Desativa slew rate control para 100 kHz
}

void I2C_Start() {
    SEN = 1; // Iniciar a condição de start
    while (SEN); // Esperar até o start ser completado
}

void I2C_Stop() {
    PEN = 1; // Iniciar a condição de stop
    while (PEN); // Esperar até o stop ser completado
}

void I2C_Write(unsigned char data) {
    SSPBUF = data; // Carregar dado no buffer
    while (BF); // Esperar até o dado ser enviado
    while (SSPCON2bits.ACKSTAT); // Verificar se houve ACK
}

unsigned char I2C_Read(unsigned char ack) {
    RCEN = 1; // Habilitar recepção
    while (!BF); // Esperar até a recepção estar completa
    unsigned char data = SSPBUF; // Ler dado recebido

    // Enviar ACK ou NACK
    if (ack) {
        ACKDT = 0; // ACK
    } else {
        ACKDT = 1; // NACK
    }
    ACKEN = 1; // Enviar ACK ou NACK
    while (ACKEN); // Esperar ACK ou NACK ser enviado

    return data;
}
