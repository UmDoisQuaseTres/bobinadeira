#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <xc.h>
#include <stdint.h>

void i2c_lcdCommand(uint8_t command);
void i2c_lcdData(uint8_t data);
void i2c_lcdXY(int8_t x, int8_t y);
void i2c_lcdText(const char *txt);
void i2c_lcdClear(void);
void i2c_lcdInit(void);

#endif /* I2C_LCD_H */
