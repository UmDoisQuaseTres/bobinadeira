
//Projeto bobinadeira autom�tica para trafos e indutores
//Autor: Matheus de Oliveira
//07-09-2024

//=============================================================================
// === Configura��es do micro

#include "config.h"

//=============================================================================
// === Botoes de interacao
#define SA RB1  //Bot�o para selecionar a op��o (a) e (-)
#define SB RB2  //Bot�o para pr�ximo menu
#define SC RB3  //Bot�o para cancelar a��o e retornar ao menu principal
#define SD RB4  //Bot�o para selecionar a op��o (b) e (+)

//=============================================================================
// === Bibliotecas

#include <xc.h>
#include "i2c_lcd.h"
#include "pcf8574.h"  // Inclua a biblioteca PCF8574
#include <stdio.h>

//=============================================================================
// === Flags auxiliares

unsigned char flag = 0x00;
unsigned char flag_more = 0x00;
unsigned char flag_minus = 0x00;

//=============================================================================
// === Variaveis globais

unsigned short screen = 0x00;  //Qual a tela atual
unsigned short previousScreen = 0x00; //Estado da tela anterior
unsigned char nScreen = 3;  //N�mero de menus
__bit backLight = 0; // Controle do backlight (0 = apagado, 1 = ligado)

//=============================================================================
// === Prototipos das funcoes
void read_buts(void);
void initialScreen(void);

//=============================================================================

int main() {
    //=============================================================================
    // === Pinos RC3 e RC4 como entrada I2C

    TRISC |= 0x18;

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
        read_buts(); // Chama fun��o para leitura dos bot�es

        // S� atualiza a tela se o valor de screen mudou
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
    // Verifica se o bot�o "menos" foi pressionado
    if (SA) {
        flag_minus = 0x01; // Seta a flag quando o bot�o "menos" for pressionado
    }
    // Verifica se o bot�o "mais" foi pressionado
    if (SB) {
        flag_more = 0x01; // Seta a flag quando o bot�o "mais" for pressionado
    }

    // Se o bot�o "menos" foi solto e a flag estava setada
    if (SA && flag_minus) {
        flag_minus = 0x00; // Limpa a flag
        screen--; // Decrementa a tela
        if (screen < 0x01) {
            screen = nScreen; // Se o valor de screen for menor que 1, retorna ao valor m�ximo
        }
        i2c_lcdClear(); // Limpa a tela, substituindo o lcd_clr
    }

    // Se o bot�o "mais" foi solto e a flag estava setada
    if (SB && flag_more) {
        flag_more = 0x00; // Limpa a flag
        screen++; // Incrementa a tela
        if (screen > nScreen) {
            screen = 0x01; // Se o valor de screen for maior que o valor m�ximo, retorna a 1
        }
        i2c_lcdClear(); // Limpa a tela, substituindo o lcd_clr
    }
}
