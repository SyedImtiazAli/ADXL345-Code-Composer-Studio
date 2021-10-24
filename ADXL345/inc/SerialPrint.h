#include "driverlib.h"
void SerialPrintln(unsigned char input);
static char *itoa_simple_helper(char *dest, int i);
char *itoa_simple(char *dest, int i);
void SerialBegin(uint32_t baudrate);
void SerialPrintString(unsigned char input);
void SerialPrint(unsigned char input);
