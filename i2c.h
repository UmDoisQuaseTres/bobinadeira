/* 
 * File:   i2c.h
 * Author: Matheus de Oliveira
 * Comments: Header file for I2C functions for PIC16F877A and PCF8574A
 * Revision history: 1.0
 */

#ifndef I2C_H
#define I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 1000000UL  // Define the system frequency for delay functions

void i2c_wait(void);
void i2c_init(long baud);
void i2c_stop(void);
void i2c_start(void);
void i2c_write(uint8_t data);
uint8_t i2c_read(char ACK);

#endif /* I2C_H */

