
/* 
 * File:   LCD Library for PIC
 * Author: electroSome
 * Comments: Algumas alterações feitas para adaptar ao meu código
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

//LCD Functions Developed by electroSome

void Lcd_Port(char a) {
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

Lcd_Clear(void) {
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x01);
}

void Lcd_Set_Cursor(char row, char column) {
    char temp, z, y;
    switch (row) {
        case 1:
            temp = 0x80 + column - 1; // Linha 1 começa em 0x80
            break;
        case 2:
            temp = 0xC0 + column - 1; // Linha 2 começa em 0xC0
            break;
        case 3:
            temp = 0x94 + column - 1; // Linha 3 começa em 0x94
            break;
        case 4:
            temp = 0xD4 + column - 1; // Linha 4 começa em 0xD4
            break;
        default:
            temp = 0x80 + column - 1; // Linha padrão (1)
            break;
    }

    z = temp >> 4;
    y = temp & 0x0F;
    Lcd_Cmd(z);
    Lcd_Cmd(y);
}

void Lcd_Init() {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(11);
    Lcd_Cmd(0x03);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x08); // Configura display de 4 linhas, 5x7 matrix
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C); // Liga o display, sem cursor
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x06); // Modo de incremento de cursor
}

void Lcd_Write_Char(char a) {
    char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    Lcd_Port(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left() {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}

void Lcd_Write_Number(unsigned char num) {
    if (num >= 100) {
        Lcd_Write_Char((num / 100) + '0'); // Centenas
    }
    if (num >= 10) {
        Lcd_Write_Char(((num % 100) / 10) + '0'); // Dezenas
    }
    Lcd_Write_Char((num % 10) + '0'); // Unidades
}


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

