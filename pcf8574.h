/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   pcf8574.h
 * Author: Admin
 * Comments: Header file for PCF8574A functions using I2C
 * Revision history: 
 */

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
