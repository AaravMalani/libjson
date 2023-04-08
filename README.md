# libjson: A light-weight feature complete JSON parser
## Installation
### Windows
Download the zip file from [the latest release](https://github.com/AaravMalani/libjson/releases/latest) and [add it to 
the Visual Studio link arguments](https://learn.microsoft.com/en-us/cpp/build/adding-references-in-visual-cpp-projects?view=msvc-170)
OR
Build from source 
```bat
git clone https://github.com/AaravMalani/libjson/
cd libjson
cmake -S . -B build
cd build
cmake --build .
cd ..
rem The .dll and .lib file is in the build directory
rem The include files are in the include directory
```
### Debian-based Linux
Download the [`.deb`](https://github.com/AaravMalani/libjson/releases/latest) file and run
```bash
sudo dpkg -i <file>
```
OR
Build from source

### Red Hat / CentOS / Fedora 
Download the [`.rpm`](https://github.com/AaravMalani/libjson/releases/latest) file and 

### Linux (From Source)
```bash
git clone https://github.com/AaravMalani/libjson/
cd libjson
cmake -S . -B build
cd build
sudo make install
cd ..
```

## Usage
```c
#include <json.h>

int main(int argc, char** argv){
    JSONElement* element = parseJSON("{\"a\":\"b\"}", NULL); // The second argument is for inner recursed calls and can therefore be NULL
    printf("%s\n", (char*) getElement(element, "a")->data); // Get inner element (char* for objects and uint64_t for arrays)
    freeJSONElement(element);
    element = parseJSON("{\"a\":[1.0, -1.4]}", NULL);
    printf("%s\n", (char*) getElements(element, 2, "a", 0)->data); // C equivalent of element["a"][0] (first argument is root element, second argument is number of indices and third argument onwards are the indices)
    freeJSONElement(element);
    
    return 0;
}
```
For all the different types, look at `include/json.h`

