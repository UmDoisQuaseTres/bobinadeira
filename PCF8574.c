/*
 * File:   pcf8574.c
 * Author: Admin
 *
 * Adapted for PIC16F877A and PCF8574A
 */

#include "pcf8574.h"
#include "i2c.h"  // Certifique-se de incluir o cabeçalho com as funções I2C

/*PCF8574A*/
#define PCF8574_W   0x70  // Endereço de escrita do PCF8574A (com pinos de endereço conectados ao GND)
#define PCF8574_R   0x71  // Endereço de leitura do PCF8574A

/**
 * @brief Envia dados ao PCF8574A via I2C.
 * 
 * @param data Byte de dados a ser enviado.
 * 
 * Esta função envia um byte de dados ao dispositivo PCF8574A utilizando
 * a interface I2C.
 */
void pcf8574Write(uint8_t data){
    i2c_start();            // Inicia a comunicação I2C
    i2c_write(PCF8574_W);   // Envia o endereço de escrita do PCF8574A
    i2c_write(data);        // Envia os dados
    i2c_stop();             // Finaliza a comunicação I2C
}

/**
 * @brief Lê dados do PCF8574A via I2C.
 * 
 * @return Retorna o byte de dados lido do dispositivo PCF8574A.
 * 
 * Esta função realiza a leitura de um byte de dados do dispositivo
 * PCF8574A utilizando a interface I2C.
 */
/*uint8_t pcf8574Read(){
    uint8_t data;  
    i2c_start();            // Inicia a comunicação I2C
    i2c_write(PCF8574_R);   // Envia o endereço de leitura do PCF8574A
    data = i2c_read(0);     // Lê o dado e envia NACK (última leitura)
    i2c_stop();             // Finaliza a comunicação I2C
    return data;            // Retorna o dado lido
}*/
