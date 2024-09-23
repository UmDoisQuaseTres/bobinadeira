

#ifndef PCF8574_H
#define PCF8574_H

#include <xc.h>
#include "i2c.h"  // Inclui o cabeçalho de funções I2C

/**
 * @brief Envia dados ao PCF8574A via I2C.
 * 
 * @param data Byte de dados a ser enviado.
 * 
 * Esta função envia um byte de dados ao dispositivo PCF8574A utilizando
 * a interface I2C.
 */
void pcf8574Write(uint8_t data);

/**
 * @brief Lê dados do PCF8574A via I2C.
 * 
 * @return Retorna o byte de dados lido do dispositivo PCF8574A.
 * 
 * Esta função realiza a leitura de um byte de dados do dispositivo
 * PCF8574A utilizando a interface I2C.
 */
uint8_t pcf8574Read();

#endif /* PCF8574_H */
