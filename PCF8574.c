/*
 * File:   pcf8574.c
 * Author: Admin
 *
 * Adapted for PIC16F877A and PCF8574A
 */

#include "pcf8574.h"
#include "i2c.h"  // Certifique-se de incluir o cabe�alho com as fun��es I2C

/*PCF8574A*/
#define PCF8574_W   0x70  // Endere�o de escrita do PCF8574A (com pinos de endere�o conectados ao GND)
#define PCF8574_R   0x71  // Endere�o de leitura do PCF8574A

/**
 * @brief Envia dados ao PCF8574A via I2C.
 * 
 * @param data Byte de dados a ser enviado.
 * 
 * Esta fun��o envia um byte de dados ao dispositivo PCF8574A utilizando
 * a interface I2C.
 */
void pcf8574Write(uint8_t data){
    i2c_start();            // Inicia a comunica��o I2C
    i2c_write(PCF8574_W);   // Envia o endere�o de escrita do PCF8574A
    i2c_write(data);        // Envia os dados
    i2c_stop();             // Finaliza a comunica��o I2C
}

/**
 * @brief L� dados do PCF8574A via I2C.
 * 
 * @return Retorna o byte de dados lido do dispositivo PCF8574A.
 * 
 * Esta fun��o realiza a leitura de um byte de dados do dispositivo
 * PCF8574A utilizando a interface I2C.
 */
/*uint8_t pcf8574Read(){
    uint8_t data;  
    i2c_start();            // Inicia a comunica��o I2C
    i2c_write(PCF8574_R);   // Envia o endere�o de leitura do PCF8574A
    data = i2c_read(0);     // L� o dado e envia NACK (�ltima leitura)
    i2c_stop();             // Finaliza a comunica��o I2C
    return data;            // Retorna o dado lido
}*/
