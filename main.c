
//Projeto bobinadeira automática para trafos e indutores
//Autor: Matheus de Oliveira
//07-09-2024

// PIC16F877A Configuration Bit Settings

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//Clock do MCU
#define _XTAL_FREQ 8000000

//Botoes de interação
#define SA RB1
#define SB RB2
#define SC RB3
#define SD RB4
#define SE RB5

//Saídas para o LCD
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//Bibliotecas
#include <xc.h>
#include "lcd.h"

//Variaveis globais
unsigned char flag = 0x00;
unsigned char rounds = 0x00;
unsigned char coreDiameter = 0x00;
unsigned char wire = 0x00;

//Protótipos das funções
void initialScreen(void);
void indutorScreen(void);
void trafoScreen(void);
void testScreen(void);
void voltasScreen(void);
void bitolaScreen(void);

int main()
{
    //Pinos RC3 e RC4 como entrada I2C
    TRISC |= 0x18;
    
   //Pinos RD2 a RD7 como saída digital
    TRISD &= 0x03;
    
    //Pinos RB1 a RB5 como enrada digital
    TRISB |= 0x3E;
    
    
    /*
    // Configuração do módulo I2C
SSPSTAT = 0x80; // Define o bit de síncrono (SMP = 1) para modo de alta velocidade
SSPCON = 0x28;  // Configura o I2C em modo mestre (SSPEN = 1 e SSPM = 8)

// Configuração da velocidade do I2C
SSPADD = 0x27; // Define o valor do registrador SSPADD para uma frequência de aproximadamente 100kHz com o Fosc de 8MHz
    */
    //Inicializando o LCD
    Lcd_Init();
    Lcd_Set_Cursor(2,5);
    Lcd_Write_String("Iniciando...");
     __delay_ms(1000);
     
     //Limpando os caracteres
    Lcd_Clear();
    __delay_ms(50);
     
    //Loop infinito
    while(1)
    {

    initialScreen();
    indutorScreen();
    trafoScreen();
    testScreen();
    
    }
    return 0;
}

//Função da tela principal
void initialScreen(){
        flag = 0x01;
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Auto Bobinadeira 1.0");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Selecione o produto:");
        Lcd_Set_Cursor(4,1);
        Lcd_Write_String("(A)Indutor");
        Lcd_Set_Cursor(4,13);
        Lcd_Write_String("(B)Trafo");
    }

//Função da tela do indutor
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
            if (SD == 0x00) coreDiameter++;
            
            // Calcular o número de dígitos
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
            
            // Tamanho do texto a ser exibido
            unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
            unsigned char lcdWidth = 20; // Número de colunas do LCD
            unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar
            
            // Ajustar a posição do cursor
            Lcd_Set_Cursor(3, startPos);
            Lcd_Write_Number(coreDiameter);
            Lcd_Set_Cursor(3, startPos + numDigits);
            Lcd_Write_String(" mm");
            __delay_ms(50);
            
            // Se o botão próximo for pressionado...
            if (SB == 0x00) {
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

    
    //Função da tela do trafo
void trafoScreen() {
    if (SB == 0x00) {
        __delay_ms(200);
        Lcd_Clear();
        while (flag == 0x01) {
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("Diametro do nucleo:");
            Lcd_Set_Cursor(4, 14);
            Lcd_Write_String("(b)Prox");
            Lcd_Set_Cursor(4, 1);
            Lcd_Write_String("(c)Canc");
            __delay_ms(100);
            
            if (SD == 0x00) coreDiameter++;
            
            // Calcular o número de dígitos
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
            
            // Tamanho do texto a ser exibido
            unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
            unsigned char lcdWidth = 20; // Número de colunas do LCD
            unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

            // Ajustar a posição do cursor
            Lcd_Set_Cursor(3, startPos);
            Lcd_Write_Number(coreDiameter);
            Lcd_Set_Cursor(3, startPos + numDigits);
            Lcd_Write_String(" mm");
            __delay_ms(50);
            
            if (SB == 0x00) {
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
        
        if (SD == 0x00) rounds++;

        // Calcular o número de dígitos
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
        
        // Tamanho do texto a ser exibido
        unsigned char textLength = numDigits + 6; // Número de dígitos + " Voltas"
        unsigned char lcdWidth = 20; // Número de colunas do LCD
        unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar

        // Ajustar a posição do cursor
        Lcd_Set_Cursor(3, startPos);
        Lcd_Write_Number(rounds);
        Lcd_Set_Cursor(3, startPos + numDigits);
        Lcd_Write_String(" Voltas");
        __delay_ms(50);

        if (SB == 0x00) {
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
    
    //Bitola AWG
void bitolaScreen() {
    flag = 0x02;
    Lcd_Clear();
    while (flag == 0x02) {
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("Qual a bitola AWG?:");
        Lcd_Set_Cursor(4, 14);
        Lcd_Write_String("(b)Prox");
        Lcd_Set_Cursor(4, 1);
        Lcd_Write_String("(c)Canc");
        __delay_ms(100);
        
        // Se SD for pressionado, incrementa a bitola
        if (SD == 0x00) wire++;
        
        // Calcular o número de dígitos
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
        
        // Tamanho do texto a ser exibido
        unsigned char textLength = numDigits + 3; // Número de dígitos + " mm"
        unsigned char lcdWidth = 20; // Número de colunas do LCD
        unsigned char startPos = (lcdWidth - textLength) / 2; // Posição inicial para centralizar
        
        // Ajustar a posição do cursor
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
/*
void I2C_Start() {
    SSPCON2bits.SEN = 1; // Envia o sinal de início
    while (SSPCON2bits.SEN); // Aguarda o término do sinal de início
}
void I2C_Stop() {
    SSPCON2bits.PEN = 1; // Envia o sinal de parada
    while (SSPCON2bits.PEN); // Aguarda o término do sinal de parada
}

*/
    
    
    
    void testScreen(){
        if(SE == 0x00){
            Lcd_Clear();
        while(1){

        }}
    }