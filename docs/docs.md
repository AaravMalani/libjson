# libjson Docs


___

## Enums

___

### JSONType types for the parser


#### Type Defenitions

    - STRING,char*
    - ARRAY,JSONElement**
    - BOOLEAN,uint64_t,
    - NUMBER,char*,
    - OBJECT,JSONMapElement*
    - NOTHINGNULL
___


## Structures


___

### JSONElementA JSON element


#### Members

    - JSONType typeThe type of the element
    - size_t lengthThe length of the element
    - void * dataThe actual data
___
___

### JSONMapElementA key-value pair for objects


#### Members

    - char *keyThe key (Always an string)
    - size_t lengthThe length of the element
    - void * dataThe actual data
___


## Functions


___

### parseJSONParses the JSON


#### Parameters

    - char *dataThe data that needed to be parsed
    - char **indexThe index of the data

#### Returns

JSONElement*The parsed JSON
___
