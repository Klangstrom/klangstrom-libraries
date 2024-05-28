# Klangstrom Libraries Coding Style

- favor `#pragma once`  over classic include guard in header files 
- parameters in function and method calls are written in lower case with underscores replacing space e.g: `void foobar(float bar_value);`


## class definitions

```
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_myperipheral_init();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class MyPeripheral {
public:
    MyPeripheral();
    virtual ~MyPeripheral();

    void init();
};
#endif // __cplusplus
```

```
MyPeripheral::MyPeripheral() {
}

MyPeripheral::~MyPeripheral() {
}

void MyPeripheral::init() {
    KLST_BSP_myperipheral_init();
}
```