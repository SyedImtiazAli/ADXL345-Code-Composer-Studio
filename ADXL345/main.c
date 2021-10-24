#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "Wire.h"
#include "SerialPrint.h"
#include "stdlib.h"
#include "string.h"

// Declaring variables
uint16_t Available_I2C[20];
uint16_t status;
uint16_t data[6];
uint16_t data2[6];
uint16_t *k = data;
char acclx[10];
char accly[10];
char acclz[10];
char carriage_1[2] = {'\r'};
char newLine[2] = {'\n'};
char comma[3] = {' , '};
int16_t xAccl,yAccl,zAccl;
// Declaration ends
Wire wire;
//
// Main
//

void main(void)
{
    Device_init();
    SerialBegin(9600);
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();

    wire.beginTransmission(83);
    wire.write(0x2C);
    wire.write(0x0A);
    wire.endTransmission();

    wire.beginTransmission(83);
    wire.write(0x2D);
    wire.write(0x08);
    wire.endTransmission();

    wire.beginTransmission(83);
    wire.write(0x31);
    wire.write(0x08);
    wire.endTransmission();

    DEVICE_DELAY_US(300000);


    while(1)
    {
        wire.requestFrom(83,50,6);
        wire.read(k,6);
        xAccl = ((((int16_t)data[1]) << 8) | (int16_t)data[0]);
                //
                //
        yAccl = ((((int16_t)data[3]) << 8) | (int16_t)data[2]);
                //
                //
        zAccl = ((((int16_t)data[5]) << 8) | (int16_t)data[4]);


        ltoa(xAccl,acclx,10);
        ltoa(yAccl,accly,10);
        ltoa(zAccl,acclz,10);

        SCI_writeCharArray(SCIA_BASE, (uint16_t*)acclx, 10);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)comma, 3);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)accly, 10);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)comma, 3);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)acclz, 10);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)newLine, 2);
        SCI_writeCharArray(SCIA_BASE, (uint16_t*)carriage_1, 2);
//        DEVICE_DELAY_US(1000);
//        DEVICE_DELAY_US(1000);
    }

}







