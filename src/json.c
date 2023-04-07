#include "../include/json.h"
void freeJSONElement(JSONElement *element)
{
    if (!element)
    {
        return;
    }
    switch (element->type)
    {
    case ARRAY:;
        if (element->data)
        {
            for (size_t i = 0; i < element->length; i++)
            {
                
                freeJSONElement(((JSONElement **)element->data)[i]);
            }
            free(element->data);
        }
        free(element);
        return;
    case OBJECT:;
        if (element->data)
        {
            for (size_t i = 0; i < element->length; i++)
            {
                freeJSONElement(((JSONMapElement *)element->data)[i].value);
                if (((JSONMapElement *)element->data)[i].key) {
                    free(
                        ((JSONMapElement *)element->data)[i].key);
                };
                
            }
            free(element->data);
        }
        
        free(element);
        return;
    case BOOLEAN:
        free(element);
        return;
    case NOTHING:
        free(element);
        return;
    
    default:
        if (element->data)
        {
            free(element->data);
        }
        free(element);
    }
}
JSONElement *parseJSON(char *data, char **index)
{
    JSONElement *element = malloc(sizeof(JSONElement));
    while (*data == ' ' || *data == '\n' || *data == '\t')
    {
        data++;
    }
    if (*data == '{')
    {
        element->type = OBJECT;
        data++;

        size_t depth = 1;
        size_t eles = 0;
        char inString = 0;
        const char *dataCopy = data;
        while (depth && *dataCopy)
        {
            if (inString && *dataCopy == '\\')
            {
                dataCopy += 2;
                continue;
            }
            if (*dataCopy == ':' && depth == 1 && !inString)
            {
                eles++;
            }
            else if (*dataCopy == '[' || *dataCopy == '{')
            {
                depth++;
            }
            else if (*dataCopy == ']' || *dataCopy == '}')
            {
                depth--;
            }
            else if (*dataCopy == '"')
            {
                inString = 1 - inString;
            }
            dataCopy++;
        }
        element->length = 0;
        element->data = malloc(sizeof(JSONMapElement) * eles);
        for (size_t i = 0; i < eles; i++)
        {
            JSONMapElement *theData = element->data;
            theData->key = NULL;
            theData->value = NULL;
            JSONElement *ele = parseJSON(data, &data);
            if (ele == NULL)
            {
                // printf("Freeing at obj -1 %.10s", data);
                freeJSONElement(element);
                return NULL;
            }
            if (ele->type != STRING)
            {
                // printf("Freeing at obj 0 %.10s", data);

                freeJSONElement(ele);
                freeJSONElement(element);
                return NULL;
            }
            
            while (*data == ' ' || *data == '\n' || *data == '\t')
            {
                data++;
            }
            if (*data != ':')
            {
                // printf("Freeing at obj 1 %.10s", data);
                freeJSONElement(ele);
                freeJSONElement(element);
                return NULL;
            }
            data++;
            JSONElement *val = parseJSON(data, &data);
            if (val == NULL)
            {
                // printf("Freeing at obj 2 %.10s", data);
                freeJSONElement(ele);
                freeJSONElement(element);
                return NULL;
            }
            // printf("%d %.10s", val->type, data);
            while (*data == ' ' || *data == '\n' || *data == '\t')
            {
                data++;
            }
            // printf("%c\n", *(data));
            if (*data != ',' && i != eles - 1)
            {
                // //printf("Freeing at obj 3 %.10s", data);
                freeJSONElement(ele);
                freeJSONElement(val);
                freeJSONElement(element);
                return NULL;
            }
            if (i != eles - 1)
            {
                data++;
            }
            theData[i].key = ele->data;
            theData[i].value = val;
            free(ele);
            element->length++;
        }
        while (*data == ' ' || *data == '\n' || *data == '\t')
        {
            data++;
        }
        data++;
    }
    else if (*data == '[')
    {
        element->type = ARRAY;
        size_t depth = 1;
        size_t eles = 0;
        char inString = 0;
        const char *dataCopy = data + 1;
        while (depth && *dataCopy)
        {

            if (inString && *dataCopy == '\\')
            {
                dataCopy += 2;
                continue;
            }
            if (*dataCopy == ',' && depth == 1 && !inString)
            {
                eles++;
            }
            else if ((*dataCopy == '[' || *dataCopy == '{') && !inString)
            {
                depth++;
            }
            else if ((*dataCopy == ']' || *dataCopy == '}') && !inString)
            {
                depth--;
            }
            else if (*dataCopy == '"')
            {
                inString = 1 - inString;
            }

            if ((*dataCopy != '\n') && (*dataCopy != ' ') && (*dataCopy != '\t') && (*dataCopy != ']') && !eles)
            {
                eles = 1;
            }
            dataCopy++;
        }
        data++;
        element->length = 0;
        element->data = malloc(sizeof(JSONElement *) * eles);
        for (size_t i = 0; i < eles; i++)
        {

            JSONElement *ele = parseJSON(data, &data);
            ((JSONElement **)element->data)[i] = NULL;
            if (ele == NULL)
            {
                // printf("Freeing at string arr 1 %.10s", data);
                freeJSONElement(element);
                return NULL;
            }
            while (*data == ' ' || *data == '\n' || *data == '\t')
            {
                data++;
            }
            if (*data != ',' && i != eles - 1)
            {
                // printf("Freeing at string arr 2 %.10s", data);
                freeJSONElement(ele);
                freeJSONElement(element);
                return NULL;
            }
            if (i != eles - 1)
            {
                data++;
            }

            ((JSONElement **)element->data)[i] = ele;
            element->length++;
        }
        // printf("%d\te", eles);
        while (*data == ' ' || *data == '\n' || *data == '\t')
        {
            data++;
        }
        data++;
    }
    else if (*data == '"')
    {
        element->type = STRING;
        size_t length = 0;
        data++;
        char *dataCopy = data;
        while (*dataCopy != '"' && *dataCopy)
        {
            if (*dataCopy == '\\')
            {
                switch (*++dataCopy)
                {
                case 'u':;
                    unsigned short arrs[] = {0x7f, 0x800, 0xffff};
                    unsigned short dat = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        if (!*++dataCopy)
                        {
                            // printf("Freeing at string 0 %.10s", data);
                            free(element);
                            return NULL;
                        }
                        if (!(('0' <= *dataCopy && *dataCopy <= '9') || ('a' <= (*dataCopy | 0x20) && (*dataCopy | 0x20) <= 'f')))
                        {
                            // printf("Freeing at string 1 %.10s", data);
                            free(element);
                            return NULL;
                        }
                        if ('0' <= *++data && '9' >= *data)
                        {
                            dat |= (*data - '0') << ((3 - i) * 4);
                        }
                        else
                        {
                            dat |= (9 + ((*data | 0x20) - 'a')) << ((3 - i) * 4);
                        }
                    }
                    int index = 0;
                    while (dat > arrs[index++])
                        ;
                    length += index;
                    break;
                case 'n':
                    length++;
                    break;
                case 'r':
                    length++;
                    break;
                case 't':
                    length++;
                    break;
                case '"':
                    length++;
                    break;
                case '\\':
                    length++;
                    break;
                case '/':
                    length++;
                    break;
                case 'b':
                    length++;
                    break;
                case 'f':
                    length++;
                    break;
                default:
                    // printf("Freeing at string 2 %.10s", data);
                    free(element);
                    return NULL;
                }
            }
            else if (*dataCopy == '\n')
            {
                // printf("Freeing at string 3 %.10s", data);
                free(element);
                return NULL;
            }
            else
            {
                length++;
            }
            dataCopy++;
        }
        if (!*dataCopy)
        {
            // printf("Freeing at dataCopy arr %.10s", data);
            free(element);
            return NULL;
        }
        element->data = malloc(length + 1);
        element->length = length;
        ((char *)element->data)[length] = 0;
        size_t index = 0;
        while (*data != '"')
        {
            if (*data == '\\')
            {
                switch (*++data)
                {
                case 'u':;
                    unsigned short arrs[] = {0x7f, 0x800, 0xffff};
                    unsigned short dat = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        if ('0' <= *++data && '9' >= *data)
                        {
                            dat |= (*data - '0') << ((3 - i) * 4);
                        }
                        else
                        {
                            dat |= (9 + ((*data | 0x20) - 'a')) << ((3 - i) * 4);
                        }
                    }
                    int idx = 0;
                    while (dat > arrs[idx++])
                        ;
                    switch (idx)
                    {
                    case 1:
                        ((unsigned char *)element->data)[index++] = dat & 0xFF;
                        break;
                    case 2:
                        ((unsigned char *)element->data)[index++] = 0b11000000 | (dat & 0b11111000000) >> 6;
                        ((unsigned char *)element->data)[index++] = 0b10000000 | dat & 0b111111;
                        break;
                    case 3:
                        ((unsigned char *)element->data)[index++] = 0b11100000 | (dat & 0b1111000000000000) >> 12;
                        ((unsigned char *)element->data)[index++] = 0b10000000 | (dat & 0b0000111111000000) >> 6;
                        ((unsigned char *)element->data)[index++] = 0b10000000 | dat & 0b0000000000111111;
                        break;
                    }
                    break;
                case 'n':
                    ((unsigned char *)element->data)[index++] = '\n';
                    break;
                case 'r':
                    ((unsigned char *)element->data)[index++] = '\r';
                    break;
                case 't':
                    ((unsigned char *)element->data)[index++] = '\t';
                    break;
                case '"':
                    ((unsigned char *)element->data)[index++] = '"';
                    break;
                case '\\':
                    ((unsigned char *)element->data)[index++] = '\\';
                    break;
                case '/':
                    ((unsigned char *)element->data)[index++] = '/';
                    break;
                case 'b':
                    ((unsigned char *)element->data)[index++] = '\b';
                    break;
                case 'f':
                    ((unsigned char *)element->data)[index++] = '\f';
                    break;
                }
            }
            else
            {
                ((unsigned char *)element->data)[index++] = *data;
            }
            data++;
        }
        data++;
    }
    else if (!strncmp(data, "true", 4))
    {
        data += 4;
        element->type = BOOLEAN;
        element->data = (void*)1;
    }
    else if (!strncmp(data, "false", 5))
    {
        data += 5;
        element->type = BOOLEAN;
        element->data = (void*)0;
    }
    else if (!strncmp(data, "null", 4))
    {
        data += 4;
        element->type = NOTHING;
    }
    else
    {
        char *copyData = data + (*data == '-');
        size_t length = (*data == '-');
        char isDecimal = 0;
        while (('0' <= *copyData && '9' >= *copyData) || (*copyData == '.' && !isDecimal))
        {
            if (*copyData == '.')
            {
                isDecimal = 1;
            }
            length++;
            copyData++;
        }
        if (length == (*data == '-'))
        {
            // printf("Freeing at num %.10s\n", data);
            free(element);
            return NULL;
        }
        element->type = NUMBER;
        element->length = length;
        element->data = malloc(length + 1);
        memcpy(element->data, data, length);
        ((char *)element->data)[length] = 0;
        data = copyData;
    }
    if (index)
    {
        *index = data;
    }
    return element;
}

JSONElement *getElement(JSONElement *parent, void *element)
{
    if (parent == NULL)
    {
        return NULL;
    }
    switch (parent->type)
    {
    case ARRAY:;
        size_t value = (size_t)element;
        if (value >= parent->length)
        {
            return NULL;
        }
        return ((JSONElement **)parent->data)[value];
    case OBJECT:;
        char *str = (char *)element;
        JSONMapElement *elements = parent->data;
        for (int i = 0; i < parent->length; i++)
        {
            if (!strcmp(str, elements[i].key))
            {
                return elements[i].value;
            }
        }
        return NULL;
    }
    return NULL;
}
JSONElement *getElements(JSONElement *element, unsigned long long no, ...)
{
    va_list lst;
    va_start(lst, no);
    for (size_t ele = 0; ele < no; ele++)
    {
        element = getElement(element, va_arg(lst, void *));
    }
    va_end(lst);
    return element;
}
