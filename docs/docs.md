# libjson Docs
___

## Types

### JSONType: Enum with the types of JSONElement
| Type | Datatype of JSONElement.data |
|---|---|
| STRING | `char*` |
| ARRAY | `JSONElement**` |
| BOOLEAN | `uint64_t` |
| NUMBER | `char*` |
| OBJECT | `JSONMapElement*` |
| NOTHING | `NULL` |
___

### JSONElement: A JSON element
| Name| Type | Description |
|---|---|---|
|type|`JSONType`|The type of the element|
|length|`size_t`|The length of the element|
|data|`void*`|The actual data (look at JSONType)|

### JSONMapElement: A key-value pair for objects
| Name| Type | Description |
|---|---|---|
|key|`char*`|The key (Always a null-terminated string)|
|value|`JSONElement*`|The value|

## Functions
___

### parseJSON: Parses the JSON

#### Parameters
| Name| Type | Description |
|---|---|---|
|data|`char*`|The data that needs to be parsed|
|index|`char**`|The index of the data|
#### Returns

`JSONElement*`: The root JSON object
___

### getElement: Get the subelement from a parent element (object[i])

#### Parameters
| Name| Type | Description |
|---|---|---|
|parent|`JSONElement*`|The parent element|
|index|`void*`|The index used to index the parent element (`uint64_t` for array and `char*` for string)|

#### Returns

`JSONElement*`: The subelement
___

### getElements: Traverse the parent element with a certain number of indices (object[i1][i2][i3])

#### Parameters
| Name| Type | Description |
|---|---|---|
|parent|`JSONElement*`|The parent element|
|no|uint64_t|The number of indices passed as an argument|
|index1|`void*`|The index used to index the parent element (`uint64_t` for array and `char*` for string)|
...repeated till indexN...

#### Returns

`JSONElement*`: The last subelement
___

### freeJSONElement - Deallocates the JSONElement and all the subelements

#### Parameters
| Name| Type | Description |
|---|---|---|
|element|`JSONElement*`|The element to free|
___


