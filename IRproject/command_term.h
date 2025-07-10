#ifndef COMMAND_TERM_H
#define COMMAND_TERM_H

#include <stdint.h>
#include <stdbool.h>

// Constants for user input
#define MAX_CHARS 80
#define MAX_FIELDS 5

//input data structure
typedef struct
{
    char buffer[MAX_CHARS + 1];  // Stores user input string (extra space for null terminator)
    uint8_t fieldCount;          // Number of parsed fields
    uint8_t fieldPosition[MAX_FIELDS]; // Start positions of fields
    char fieldType[MAX_FIELDS];   // Field types: 'a' for alphabetic, 'n' for numeric
} USER_DATA;


void getsUart0(USER_DATA *data);
void parseFields(USER_DATA *data);
char* getFieldString(USER_DATA *data, uint8_t fieldNumber);
int32_t getFieldInteger(USER_DATA *data, uint8_t fieldNumber);
bool isCommand(USER_DATA *data, const char strCommand[], uint8_t minArguments);

#endif
