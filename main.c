// Projeto bobinadeira automática para trafos e indutores
// Autor: Matheus de Oliveira
// Início: 07-09-2024
// Rev: 22-09-2024
// Adicionado controle para motor de passo

//=============================================================================
// === Configurações do micro

#include "config.h"

//=============================================================================
// === Bibliotecas

#include <xc.h>
#include "i2c_lcd.h"  //Biblioteca I2C
#include "pcf8574.h"  // Inclua a biblioteca PCF8574
#include "delaytmr.h"
#include <stdio.h>

//=============================================================================
// === Flags auxiliares

unsigned short screen = 0x01; // Qual a tela atual
unsigned short previousScreen = 0x00; // Estado da tela anterior

//=============================================================================
// === Variaveis globais

unsigned char diameter = 0x00;
unsigned char rounds = 0x00;
uint8_t backLight = 0; // Controle do backlight (0 = apagado, 1 = ligado)

//Variaveis de apoio para conversão de char
char roundsStr[5]; // Buffer para armazenar a string das laps (incluindo '\0')
char diameterStr[5]; // Buffer para armazenar a string do diâmetro (incluindo '\0')

//=============================================================================
// === Prototipos das funcoes

void initialScreen(void);
void core(void);
void laps(void);
void readButtons(void);
void step1(void);
void step2(void);
void step3(void);
void step4(void);
void execution(void);
void execTask(void);

//=============================================================================

int main() {
    // Pinos RC3 e RC4 como entrada I2C
    TRISC |= 0x18;

    //PORTD como saida
    TRISD = 0x10;

    // Pinos RB1 a RB5 como entrada digital
    TRISB |= 0x3E;

    // Ativa os pull-ups internos do PORTB
    INTCON2bits.RBPU = 0; // Ativa os pull-ups internos (RBPU = 0)

    //=============================================================================
    // === Inicializando o sistema

    i2c_lcdInit(); // Inicializa o LCD via I2C
    backLight = 1; // Liga o backlight do LCD
    delay_ms(10); // Atraso de 1 segundo

    // Exibe mensagens iniciais no LCD
    i2c_lcdXY(2, 1); // Posicionando cursor
    i2c_lcdText("Bobinadeira 0.0.1");
    i2c_lcdXY(3, 3); // Posicionando cursor
    i2c_lcdText(" Init system....");
    delay_ms(1000); // Aguardo...
    i2c_lcdClear(); // Limpa o LCD

    // Loop principal

    while (1) {

        //Carregando funções de leitura de botões
        readButtons();


        //=============================================================================
        // === Inicio do switch para selecionar as telas

        // Só atualiza a tela se o valor de screen mudou
        if (screen != previousScreen) {
            i2c_lcdClear(); // Limpa a tela
            switch (screen) {
                case 0x01:
                    initialScreen(); // Mostra a tela inicial
                    break;
                case 0x02:
                    core();
                    break;
                case 0x03:
                    core();
                    break;
                case 0x04:
                    laps();
                    break;
                case 0x05:
                    execution();
                    execTask();
                    break;
                default:
                    i2c_lcdText("Menu Invalido");
                    break;
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

//=============================================================================
// === Menu de Núcleo

void core() {
    // Converte o valor de diameter para uma string
    sprintf(diameterStr, "%u", diameter);
    i2c_lcdText("Tamanho do nucleo:");
    i2c_lcdXY(8, 2);
    i2c_lcdText(diameterStr);
    i2c_lcdXY(10, 2);
    i2c_lcdText(" mm");
    i2c_lcdXY(1, 3);
    i2c_lcdText("(D)+");
    // i2c_lcdXY(1, 4);
    // i2c_lcdText("(A)Canc");
    i2c_lcdXY(14, 3);
    i2c_lcdText("(E) -");
    i2c_lcdXY(1, 4);
    i2c_lcdText("(C) Canc");
    i2c_lcdXY(14, 4);
    i2c_lcdText("(B)Prox");
}

//=============================================================================
// === Menu de Voltas

void laps() {
    // Converte o valor de rounds para uma string
    sprintf(roundsStr, "%u", rounds);
    i2c_lcdText("Quantas voltas:");
    i2c_lcdXY(8, 2);
    i2c_lcdText(roundsStr);
    i2c_lcdXY(10, 2);
    i2c_lcdText(" voltas");
    i2c_lcdXY(1, 3);
    i2c_lcdText("(D)+");
    //i2c_lcdXY(1, 4);
    // i2c_lcdText("(A)Voltar");
    i2c_lcdXY(14, 3);
    i2c_lcdText("(E) -");
        i2c_lcdXY(1, 4);
    i2c_lcdText("(C) Canc");
    i2c_lcdXY(14, 4);
    i2c_lcdText("(B)Exec");
}

void execution() {
    i2c_lcdClear();
    sprintf(roundsStr, "%u", rounds);
    i2c_lcdText("Voltas restantes: ");
    i2c_lcdXY(10,2);
    i2c_lcdText(roundsStr);
    i2c_lcdXY(2,4);
    i2c_lcdText("Em andamento...");
    i2c_lcdXY(14, 3);
    i2c_lcdText("(C)Canc");
}

//=============================================================================
// === Leitura dos botões

void readButtons() {

    //Botão de incremento
    // Verifica se o botão SE está pressionado
    if (SE == 0x00) {
        if (screen == 0x02 || screen == 0x03) {
            diameter++; // Incrementa o valor de diameter
            if (diameter > 20) { // Limita o valor máximo de diameter se necessário
                diameter = 0;
            }
            core(); // Atualiza a tela após o incremento
        } else if (screen == 0x04) {
            rounds++; // Incrementa o valor de rounds para a tela 0x04
            if (rounds > 20) { // Limita o valor máximo de rounds se necessário
                rounds = 0;
            }
            laps(); // Atualiza a tela após o incremento
        }
    }

    //Botão de decremento
    // Verifica se o botão SF está pressionado
    if (SF == 0x00) {
        if (screen == 0x02 || screen == 0x03) {
            if (diameter > 0) {
                diameter--; // Decrementa o valor de diameter
            } else {
                diameter = 20; // Define o valor máximo se chegar a 0
            }
            core(); // Atualiza a tela após o decremento
        } else if (screen == 0x04) {
            if (rounds > 0) {
                rounds--; // Decrementa o valor de rounds para a tela 0x04
            } else {
                rounds = 20; // Define o valor máximo se chegar a 0
            }
            laps(); // Atualiza a tela após o decremento
        }
    }

    //Botão para passar de tela
    // Verifica se o botão SB está pressionado
    if (SB == 0x00) {

        if (screen == 0x02) {
            screen += 2; // Incrementa o valor de screen em 2 se for igual a 0x02
        } else {
            screen++; // Caso contrário, incrementa normalmente
        }
    }

    //Botão de cancelar
    if (SC == 0x00) { // Pressionar o SC volta ao início e zera as flags
        screen = 0x01;
        diameter = 0x00;
        rounds = 0x00;
    }


    if (SB == 0x00 && screen == 0x01) screen = 0x02; // Se estiver na tela inicial e apertar SB, vai para Trafo
    if (SA == 0x00 && screen == 0x01) screen = 0x03; // Se estiver na tela inicial e apertar SA, vai para Indutor
}

//=============================================================================
// === Comandos do motor


/* Funções para os passos do motor */

void step1() {
    mla = 0x01;
    mlb = 0x01;
    mlc = 0x00;
    mld = 0x00;
}

void step2() {
    mla = 0x00;
    mlb = 0x01;
    mlc = 0x01;
    mld = 0x00;
}

void step3() {
    mla = 0x00;
    mlb = 0x00;
    mlc = 0x01;
    mld = 0x01;
}

void step4() {
    mla = 0x01;
    mlb = 0x00;
    mlc = 0x00;
    mld = 0x01;
}

/* FIM DAS Funções para os passos do motor */

/*Executa os passos do motor de acordo com o numero de voltas, sabendo que ainda
 * não fizemos os calculos para saber quantos passos serão necessários para dar
 * uma volta */

void execTask() {
    while (rounds > 0x00) {
        step1();
        delay_ms(10);
        step2();
        delay_ms(10);
        step3();
        delay_ms(10);
        step4();
        delay_ms(10);
        if (SNS == 0x00) {
            rounds--;
            execution();
        }
        if(!rounds) PORTD = 0x00;
    } 
}