#include <xc.h>
#include "i2c.h"

void i2c_wait(void) {
    while (PIR1bits.SSPIF == 0); // Aguarda o final da transmiss�o
    PIR1bits.SSPIF = 0; // Limpa o flag de interrup��o
}

void i2c_init(long baud) {
    TRISC3 = 1; // SCL como entrada
    TRISC4 = 1; // SDA como entrada
    SSPCON1 = 0x28; // Modo I2C Master, clock = Fosc / (4 * (SSPADD + 1))
    SSPSTAT = 0x00; // Slew rate disabled para opera��o em 100kHz
    SSPADD = (unsigned char)((8000000 / (4 * baud)) - 1); // Calcula o valor de baud rate
}

void i2c_stop(void) {
    SSPCON2bits.PEN = 1; // Envia condi��o de parada
    i2c_wait(); // Aguarda a condi��o de parada ser completada
}

void i2c_start(void) {
    SSPCON2bits.SEN = 1; // Envia condi��o de start
    i2c_wait(); // Aguarda a condi��o de start ser completada
}

void i2c_write(uint8_t data) {
    SSPBUF = data; // Coloca o dado no buffer de transmiss�o
    i2c_wait(); // Aguarda a transmiss�o
    if (SSPCON2bits.ACKSTAT == 1) { // Se n�o houve ACK
        i2c_stop(); // Envia condi��o de stop
    }
}

uint8_t i2c_read(char ACK) {
    uint8_t temp;
    SSPCON2bits.RCEN = 1; // Habilita recep��o
    i2c_wait(); // Aguarda o dado ser recebido
    temp = SSPBUF; // L� o dado recebido
    if (ACK == 0)
        SSPCON2bits.ACKDT = 1; // N�o enviar ACK (NACK)
    else
        SSPCON2bits.ACKDT = 0; // Enviar ACK
    SSPCON2bits.ACKEN = 1; // Envia o sinal de ACK ou NACK
    i2c_wait(); // Aguarda o t�rmino
    return temp; // Retorna o dado recebido
}
