#include "command_term.h"
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "uart0.h"


void getsUart0(USER_DATA *data)
{
    uint8_t count = 0;
    char c;

    while (1)
    {
        c = getcUart0();
        //Backspace
        if (c == 8 || c == 127)
        {
            if (count > 0)
                count--; //remove last char
        }
        else if (c == 13) //enter key or carriage return
        {
            data->buffer[count] = '\0'; //Null-terminate string
            return;
        }
        else if (c >= 32) //printable char
        {
            if (count < MAX_CHARS)
            {
                data->buffer[count] = c;
                count++;
            }
        }
    }
}
//parseFields function
void parseFields(USER_DATA *data)
{
    //get length of input
    int origLen = 0;
    while (data->buffer[origLen] != '\0')
    {
        origLen++;
    }

    uint8_t i = 0;
    //field index
    uint8_t field = 0;
    //'a' for alpha, 'n' for numeric, 'd' for delimiter
    char prevType = 'd';
    //process first character
    if (origLen > 0)
    {
        char c = data->buffer[0];
        uint8_t isAlpha = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        uint8_t isNumeric = (c >= '0' && c <= '9') || (c == '-') || (c == '.');
        if (isAlpha || isNumeric)
        {
            data->fieldPosition[0] = 0;
            data->fieldType[0] = isAlpha ? 'a' : 'n';
            prevType = data->fieldType[0];
        }
    }
    //process rest of characters
    while (i < origLen && data->buffer[i] != '\0')
    {
        char c = data->buffer[i];
        uint8_t isAlpha = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        uint8_t isNumeric = (c >= '0' && c <= '9') || (c == '-') || (c == '.');
        uint8_t isDelimiter = (!isAlpha && !isNumeric);

        if (isDelimiter) {
            //replace delimiter with null terminator
            data->buffer[i] = '\0';
            prevType = 'd';
        }
        else
        {
            char currentType = isAlpha ? 'a' : 'n';
            //if previous character was a delimiter start new field
            if (prevType == 'd')
            {
                //first field already handled
                if (i != 0) {
                    field++;
                    if (field < MAX_FIELDS)
                    {
                        data->fieldPosition[field] = i;
                        data->fieldType[field] = currentType;
                    } else
                    {
                        data->buffer[i] = '\0';
                        data->fieldCount = MAX_FIELDS;
                        return;
                    }
                }
                prevType = currentType;
            }
            //don't know if I need this but type change from letter the char or vice verse eg- "abc123"
            else if (currentType != prevType)
            {
                //we need to shift the buffer to the right from that point so we can put a delimiter between them,
                //so we need to check if we got space
                if (origLen < MAX_CHARS)
                {
                    int32_t j;
                    //shift
                    for (j = origLen; j >= i; j--)
                    {
                        data->buffer[j+1] = data->buffer[j];
                    }
                    //put null at that spot and increase length
                    data->buffer[i] = '\0';
                    origLen++;

                    //record new field at i+1.
                    field++;
                    if (field < MAX_FIELDS)
                    {
                        data->fieldPosition[field] = i + 1;
                        data->fieldType[field] = currentType;
                    } else
                    {
                        data->buffer[i] = '\0';
                        data->fieldCount = MAX_FIELDS;
                        return;
                    }
                    prevType = currentType;
                    i++; //skip over the null terminator we added
                    continue;
                } else
                {
                    // If no room to shift
                    data->buffer[i] = '\0';
                    prevType = 'd';
                }
            } else
            {
                prevType = currentType;
            }
        }
        i++;
    }
    data->fieldCount = field + 1;
}
//----------------------------------------------------
//
//get field string value
char* getFieldString(USER_DATA *data, uint8_t fieldNumber)
{
    if (fieldNumber < data->fieldCount)
    {
        return &data->buffer[data->fieldPosition[fieldNumber]];
    }
    else
    {
        return "\0";
    }
}

//get a field integer value
int32_t getFieldInteger(USER_DATA *data, uint8_t fieldNumber)
{
    //check if in field range
    if (fieldNumber >= data->fieldCount)
    {
        return 0;
    }
    if (data->fieldType[fieldNumber] != 'n')
    {
        return 0;
    }
    char *s = &data->buffer[data->fieldPosition[fieldNumber]];
    int32_t result = 0;
    uint8_t negative = 0;

    //check for a negative
    if (*s == '-')
    {
        negative = 1;
        s++;
    }
    //convert to integer
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    if (negative)
    {
        result = -result;
    }
    return result;
}


//check if the command matches and has enough arguments
bool isCommand(USER_DATA *data, const char strCommand[], uint8_t minArguments)
{
    // There must be at least one field.
    if (data->fieldCount == 0)
    {
        return false;
    }

    //check if has minArguments
    if ((data->fieldCount - 1) < minArguments)
    {
        return false;
    }

    //compare the first field with string
    char *cmd = &data->buffer[data->fieldPosition[0]];
    int32_t i = 0;
    while (strCommand[i] != '\0' && cmd[i] != '\0')
    {
        char c1 = strCommand[i];
        char c2 = cmd[i];
        //make lowercase for case
        if (c1 >= 'A' && c1 <= 'Z')
            c1 = c1 + 32;
        if (c2 >= 'A' && c2 <= 'Z')
            c2 = c2 + 32;
        if (c1 != c2)
            return false;
        i++;
    }
    if (strCommand[i] == '\0' && cmd[i] == '\0')
    {
        return true;
    }
    else
    {
        return false;
    }
}
