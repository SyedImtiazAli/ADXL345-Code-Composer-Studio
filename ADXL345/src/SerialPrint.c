#include "stdio.h"
#include "stdlib.h"
#include <limits.h>
#include <SerialPrint.h>
#include "string.h"
#include "driverlib.h"
#include "device.h"
unsigned char s[100];
unsigned char *msg;
unsigned char carriage = '\r';
unsigned char feed = '\n';
unsigned char space = '  ';
uint16_t size = 0;

void SerialBegin(uint32_t baudrate)
{
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, baudrate, (SCI_CONFIG_WLEN_8 |
    SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE));
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

#ifdef AUTOBAUD
        //
        // Perform an autobaud lock.
        // SCI expects an 'a' or 'A' to lock the baud rate.
        //
      SCI_lockAutobaud(SCIA_BASE);
     #endif

}

//void SerialPrintln(unsigned char input)
//{
//    itoa_simple(s, input); /// for now  I have limited the input to Int. I may modify it.
//
//    strncat(s, &carriage, 1);
//    strncat(s, &feed, 1);
//    size = strlen(s);
//    msg = s;
//    SCI_writeCharArray(SCIA_BASE, (uint16_t*) msg, size);
//}
//
//void SerialPrint(unsigned char input)
//{
//    itoa_simple(s, input); /// for now  I have limited the input to Int. I may modify it.
//    strncat(s, &space, 1);
//    size = strlen(s);
//    msg = s;
//    SCI_writeCharArray(SCIA_BASE, (uint16_t*) msg, size);
//}
//
//void SerialPrintString(unsigned char input)
//{
//    msg = input;
//    size = 100;
//    SCI_writeCharArray(SCIA_BASE, (uint16_t*) msg, size);
//}
//
//static char* itoa_simple_helper(char *dest, int i)
//{
//    if (i <= -10)
//    {
//        dest = itoa_simple_helper(dest, i / 10);
//    }
//    *dest++ = '0' - i % 10;
//    return dest;
//}
//
//char* itoa_simple(char *dest, int i)
//{
//    char *s = dest;
//    if (i < 0)
//    {
//        *s++ = '-';
//    }
//    else
//    {
//        i = -i;
//    }
//    *itoa_simple_helper(s, i) = '\0';
//    return dest;
//}
