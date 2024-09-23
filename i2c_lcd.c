#include "config.h"
#include "i2c_lcd.h"
#include "pcf8574.h"
#include "delaytmr.h"
#include <stdio.h>

extern uint8_t backLight; // Variável que controla o estado do backlight

void i2c_lcdCommand(uint8_t command) {
    uint8_t upper_nibble = command & 0xF0;
    uint8_t lower_nibble = (command << 4) & 0xF0;

    pcf8574Write((uint8_t)(upper_nibble | (backLight << BL) | (1 << EN)));
    __delay_us(10);
    pcf8574Write((uint8_t)(upper_nibble | (backLight << BL)));
    __delay_us(50);

    pcf8574Write((uint8_t)(lower_nibble | (backLight << BL) | (1 << EN)));
    __delay_us(10);
    pcf8574Write((uint8_t)(lower_nibble | (backLight << BL)));
    __delay_us(50);
}

void i2c_lcdData(uint8_t data) {
    uint8_t upper_nibble = data & 0xF0;
    uint8_t lower_nibble = (data << 4) & 0xF0;

    pcf8574Write((uint8_t)(upper_nibble | (backLight << BL) | (1 << EN) | (1 << RS)));
    __delay_us(10);
    pcf8574Write((uint8_t)(upper_nibble | (backLight << BL) | (1 << RS)));
    __delay_us(50);

    pcf8574Write((uint8_t)(lower_nibble | (backLight << BL) | (1 << EN) | (1 << RS)));
    __delay_us(10);
    pcf8574Write((uint8_t)(lower_nibble | (backLight << BL) | (1 << RS)));
    __delay_us(50);
}

void i2c_lcdXY(int8_t x, int8_t y) {
    int8_t addr[] = {(int8_t)0x80, (int8_t)0xC0, (int8_t)0x94, (int8_t)0xD4};
    if (y >= 1 && y <= 4 && x >= 1 && x <= 20) {
        i2c_lcdCommand((uint8_t)(addr[y - 1] + (x - 1)));
    }
}

void i2c_lcdText(const char *txt) {
    while (*txt) {
        i2c_lcdData(*txt++);
    }
}

void i2c_lcdClear(void) {
    i2c_lcdCommand(0x01); 
    delay_ms(10); 
}

void i2c_lcdInit(void) {
    i2c_init(100000); 
    __delay_us(10);
    pcf8574Write(0);
    delay_ms(10);
    i2c_lcdCommand(0x33); 
    __delay_us(10);
    i2c_lcdCommand(0x32); 
    __delay_us(10);
    i2c_lcdCommand(0x28); 
    __delay_us(10);
    i2c_lcdCommand(0x0C); // Desativa o cursor e o cursor piscante
    __delay_us(10);
    i2c_lcdCommand(0x01); 
    delay_ms(10);
    i2c_lcdCommand(0x06); 
    __delay_us(10);
}
