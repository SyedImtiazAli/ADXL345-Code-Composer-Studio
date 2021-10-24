/*
 * Wire.h
 *
 *  Created on: Oct 22, 2021
 *      Author: Syed Imtiaz Ali Shah
 */

#ifndef INC_WIRE_H_
#define INC_WIRE_H_
#include "device.h"

#define ERROR_BUS_BUSY              0x1000
#define ERROR_NACK_RECEIVED         0x2000
#define ERROR_ARBITRATION_LOST      0x3000
#define ERROR_STOP_NOT_READY        0x5555
#define SUCCESS                     0x0000

extern uint16_t status;

class Wire
{
public:
    void I2CBUS_SCAN(uint16_t *AvailableDevices);
    void beginTransmission(uint16_t slaveAddr);
    void write(uint16_t data);
    void endTransmission();
    void requestFrom(uint16_t slaveAddr,uint16_t regAddr, uint16_t count);
    void read(uint16_t *data, uint16_t count);
    uint16_t handleNACK(uint32_t base);
};




#endif /* INC_WIRE_H_ */
