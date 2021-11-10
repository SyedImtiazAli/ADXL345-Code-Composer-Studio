/*
 * Wire.cpp
 *
 *  Created on: Oct 22, 2021
 *      Author: Syed Imtiaz Ali Shah
 */

#include "Wire.h"

void Wire::I2CBUS_SCAN(uint16_t *AvailableDevices)
{
    // Disable interrupt
    I2C_disableInterrupt(I2CA_BASE,(I2C_INT_ADDR_SLAVE | I2C_INT_STOP_CONDITION | I2C_INT_ARB_LOST| I2C_INT_NO_ACK));
    uint16_t Slave_Address, i;
    i = 0;
    // Maximum of 128 I2C devices can be connected carrying 7 bit address

    for (Slave_Address = 1; Slave_Address < 128; Slave_Address++)
    {
        // Check bus status, successfully it will return 0. Stop program if not equal to 0.

        while(I2C_isBusBusy(I2CA_BASE));


        // Configure I2C
        I2C_setConfig(I2CA_BASE, I2C_MASTER_SEND_MODE | I2C_REPEAT_MODE);
        I2C_setAddressMode(I2CA_BASE, I2C_ADDR_MODE_7BITS);
        I2C_setSlaveAddress(I2CA_BASE, Slave_Address);
        I2C_sendStartCondition(I2CA_BASE);
        // Wait for transmission to be completed
        // The status should be "Register-access-ready" to proceed

        while (!(I2C_getStatus(I2CA_BASE) & I2C_STS_REG_ACCESS_RDY));
        I2C_sendStopCondition(I2CA_BASE);
        // Wait for the stop bit to be cleared
        while (I2C_getStopConditionStatus(I2CA_BASE));
        // Wait for busy bit to be cleared. Bus should not be busy
        while (I2C_isBusBusy(I2CA_BASE));
        // get I2C status to see if it is acknowledged by slave
        status = I2C_getStatus(I2CA_BASE);

        if (!(status & I2C_STS_NO_ACK))
        {
            AvailableDevices[i] = Slave_Address;
            i++;
        }
        I2C_clearStatus(I2CA_BASE,I2C_STS_NO_ACK | I2C_STS_ARB_LOST | I2C_STS_REG_ACCESS_RDY| I2C_STS_STOP_CONDITION);
    }
    I2C_setConfig(I2CA_BASE, (I2C_MASTER_SEND_MODE));
    I2C_enableInterrupt(I2CA_BASE,(I2C_INT_ADDR_SLAVE | I2C_INT_STOP_CONDITION | I2C_INT_ARB_LOST| I2C_INT_NO_ACK));
}

void Wire::beginTransmission(uint16_t slaveAddr)
{
    while (I2C_getStopConditionStatus(I2CA_BASE));
    I2C_setConfig(I2CA_BASE, (I2C_MASTER_SEND_MODE|I2C_REPEAT_MODE));
    I2C_setSlaveAddress(I2CA_BASE, slaveAddr);
    I2C_sendStartCondition(I2CA_BASE);
    while(!(I2C_getInterruptSource(I2CA_BASE)&&3));
}

void Wire::write(uint16_t data)
{
    while(!(I2C_getInterruptSource(I2CA_BASE)&&5));
    I2C_putData(I2CA_BASE, data);
}

void Wire::requestFrom(uint16_t slaveAddr,uint16_t regAddr, uint16_t count)
{
    while (I2C_getStopConditionStatus(I2CA_BASE));
    I2C_setSlaveAddress(I2CA_BASE,slaveAddr);
    I2C_setConfig(I2CA_BASE, (I2C_MASTER_SEND_MODE|I2C_REPEAT_MODE));
    I2C_sendStartCondition(I2CA_BASE);
    while(!(I2C_getInterruptSource(I2CA_BASE)&3));
    I2C_putData(I2CA_BASE, regAddr);
    while(!(I2C_getInterruptSource(I2CA_BASE)&5));
    I2C_sendStopCondition(I2CA_BASE);
    while (I2C_getStopConditionStatus(I2CA_BASE));
    I2C_setSlaveAddress(I2CA_BASE,slaveAddr);
    I2C_setDataCount(I2CA_BASE, count);
    I2C_setConfig(I2CA_BASE, (I2C_MASTER_RECEIVE_MODE));
    I2C_sendStartCondition(I2CA_BASE);
}

void Wire::read(uint16_t *data, uint16_t count)
{
    while(!(I2C_getInterruptSource(I2CA_BASE)&5));
    for (int i = 0; i < count; i++)
    {
        while(!(I2C_getInterruptSource(I2CA_BASE)&4));
        data[i] = I2C_getData(I2CA_BASE);
    }
}

void Wire::endTransmission()
{
    while(!(I2C_getInterruptSource(I2CA_BASE)&5));
    I2C_sendStopCondition(I2CA_BASE);
}

uint16_t Wire::handleNACK(uint32_t base)
{
    if(I2C_getStatus(base) & I2C_STS_NO_ACK)
    {
        I2C_clearStatus(base, I2C_STS_NO_ACK);
        I2C_sendStopCondition(base);

        return ERROR_NACK_RECEIVED;
    }

    return SUCCESS;
}
