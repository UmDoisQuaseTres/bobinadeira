#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <xc.h>
#include <stdint.h>

// Definições dos pinos no PCF8574A para controlar o LCD
#define RS  0  // Pino RS do LCD (Registrador de dados ou comando)
#define RW  1  // Pino RW do LCD (Leitura/Escrita)
#define EN  2  // Pino Enable do LCD
#define BL  3  // Pino Backlight do LCD (opcional)

void i2c_lcdCommand(uint8_t command);
void i2c_lcdData(uint8_t data);
void i2c_lcdXY(int8_t x, int8_t y);
void i2c_lcdText(const char *txt);
void i2c_lcdClear(void);
void i2c_lcdInit(void);

#endif /* I2C_LCD_H */
