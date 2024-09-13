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
 * File:   i2c.h
 * Author: Admin
 * Comments: Header file for I2C functions for PIC16F877A
 * Revision history: 
 */

#ifndef I2C_H
#define I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 8000000UL  // Define the system frequency for delay functions

/**
 * @brief Waits for the I2C operation to complete.
 * 
 * This function waits for the SSPIF flag to indicate that the I2C operation
 * has completed.
 */
void i2c_wait(void);

/**
 * @brief Initializes the I2C module.
 * 
 * @param baud The desired baud rate for I2C communication.
 * 
 * This function configures the I2C module as a master and sets the baud rate
 * according to the provided value.
 */
void i2c_init(long baud);

/**
 * @brief Sends a stop condition on the I2C bus.
 * 
 * This function sends a stop condition and waits for it to be completed.
 */
void i2c_stop(void);

/**
 * @brief Sends a start condition on the I2C bus.
 * 
 * This function sends a start condition and waits for it to be completed.
 */
void i2c_start(void);

/**
 * @brief Writes data to the I2C bus.
 * 
 * @param data The data byte to be written to the I2C bus.
 * 
 * This function writes a byte of data to the I2C bus and waits for the operation
 * to complete. If an acknowledgment is not received, it sends a stop condition.
 */
void i2c_write(uint8_t data);

/**
 * @brief Reads data from the I2C bus.
 * 
 * @param ACK A flag indicating whether to send an acknowledgment (1) or not (0).
 * @return The data byte read from the I2C bus.
 * 
 * This function reads a byte of data from the I2C bus. Depending on the ACK flag,
 * it either sends an acknowledgment or a no-acknowledgment.
 */
uint8_t i2c_read(char ACK);

#endif /* I2C_H */

