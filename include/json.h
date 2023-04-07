#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
//  TYPE        TYPEOF DETAILS
    STRING,  // char*
    ARRAY,   // JSONElement**
    BOOLEAN, // uint64_t,
    NUMBER,  // char*,
    OBJECT,  // JSONMapElement*
    NOTHING  // NULL
} JSONType;
typedef struct
{
    JSONType type; // The type of the element
    size_t length; // Not necessary this will be set
    void *data;    // The actual data based on above enum
} JSONElement;

typedef struct
{
    char *key;          // Key is always string
    JSONElement *value; // The value JSON element
} JSONMapElement; // A key-value pair for objects

void freeJSONElement(JSONElement* element);
JSONElement *parseJSON(char *data, char **index);
JSONElement *getElement(JSONElement *parent, void *element);
JSONElement *getElements(JSONElement *element, unsigned long long no, ...);

#ifdef __cplusplus
}
#endif