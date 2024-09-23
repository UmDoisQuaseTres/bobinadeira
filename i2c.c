#include <xc.h>
#include "i2c.h"

void i2c_wait(void) {
    while (PIR1bits.SSPIF == 0); // Aguarda o final da transmissão
    PIR1bits.SSPIF = 0; // Limpa o flag de interrupção
}

void i2c_init(long baud) {
    TRISC3 = 1; // SCL como entrada
    TRISC4 = 1; // SDA como entrada
    SSPCON1 = 0x28; // Modo I2C Master, clock = Fosc / (4 * (SSPADD + 1))
    SSPSTAT = 0x00; // Slew rate disabled para operação em 100kHz
    SSPADD = (unsigned char)((8000000 / (4 * baud)) - 1); // Calcula o valor de baud rate
}

void i2c_stop(void) {
    SSPCON2bits.PEN = 1; // Envia condição de parada
    i2c_wait(); // Aguarda a condição de parada ser completada
}

void i2c_start(void) {
    SSPCON2bits.SEN = 1; // Envia condição de start
    i2c_wait(); // Aguarda a condição de start ser completada
}

void i2c_write(uint8_t data) {
    SSPBUF = data; // Coloca o dado no buffer de transmissão
    i2c_wait(); // Aguarda a transmissão
    if (SSPCON2bits.ACKSTAT == 1) { // Se não houve ACK
        i2c_stop(); // Envia condição de stop
    }
}

uint8_t i2c_read(char ACK) {
    uint8_t temp;
    SSPCON2bits.RCEN = 1; // Habilita recepção
    i2c_wait(); // Aguarda o dado ser recebido
    temp = SSPBUF; // Lê o dado recebido
    if (ACK == 0)
        SSPCON2bits.ACKDT = 1; // Não enviar ACK (NACK)
    else
        SSPCON2bits.ACKDT = 0; // Enviar ACK
    SSPCON2bits.ACKEN = 1; // Envia o sinal de ACK ou NACK
    i2c_wait(); // Aguarda o término
    return temp; // Retorna o dado recebido
}
