# klangstrom-libraries

all libraries are maintained in one repository called `klangstrom-libraries` that can be cloned as a submodule into architectures:

```
.
├── Klangstrom
├── KlangWellen
├── ...
└── USBHost
```

## Klangstrom

==@todo==
```
### File Naming Conventions

- `BSP` == Board Specific  > `KlangstromApplication_KLST_SHEEP-BSP`
- `SAB` == Shared Across Boards ( within an Architexture ) > `Klangstrom_ARDUINO-SAB.cpp`
```

==@todo("Klangstrom requires to set the environment in `KLST_ENV` at compile time. `KLST_ENV` defines architecture (`KLST_ARCH`) and board (`KLST_BOARD`):")==

```
KLST_ENV = KLST_ARCH + KLST_BOARD

.
├── KLST_ARCH                       (=0x10)
│   ├── KLST_ARCH_STM32             (=0x20) (Arduino,MCU)
│   ├── KLST_ARCH_EMU               (=0x30) (Arduino,CPU)
│   └── KLST_ARCH_WORMS             (=0x40) (STM32CubeIDE)
└── KLST_BOARD                      (=0x01)
    ├── KLST_BOARD_KLST_CORE        (=0x02) (STM32H743+WM8731)
    ├── KLST_BOARD_KLST_TINY        (=0x03) (STM32F446+WM8731)
    ├── KLST_BOARD_KLST_SHEEP       (=0x04) (STM32H743+WM8731)
    ├── KLST_BOARD_KLST_PANDA       (=0x05) (STM32H723+WM8904)
    └── KLST_BOARD_KLST_CATERPILLAR (=0x06) (STM32H723+WM8904)
```

the following prepocessor conditionals can be used:

```
#if ((KLST_ENV & KLST_ARCH) == KLST_ARCH_STM32)
// running on STM32
#endif

#if ((KLST_ENV & KLST_BOARD) == KLST_BOARD_KLST_PANDA)
// running on KLST_PANDA
#endif
```

there are common files ( e.g `Klangstrom.cpp` ) and board-specific (BSP) files ( `Klangstrom-BSP-KLST_PANDA.cpp` ). the common files are used by all boards and architectures.

- `Klangstrom.h`
- `Klangstrom.cpp`
- `Klangstrom-KLST_PANDA.h`
- `Klangstrom-KLST_PANDA.cpp`
- `Klangstrom-Display.h`
- `Klangstrom-Display.cpp`
- `Klangstrom-KLST_PANDA-Display.h`
- `Klangstrom-KLST_PANDA-Display.cpp`
- ...

### architectures ==@todo==

available architectures are:

- SOFT/DESKTOP @TODO(need to decide on name)
- METAL/STM32 @TODO(need to decide on name)

programatically identifiable e.g

```
#if (KLST_ARCHITECTURE_METAL)
#endif // KLST_ARCHITECTURE_METAL
```

### boards ==@todo==

available boards are for `METAL`:

- `KLST_PANDA`
- `KLST_SHEEP`
- `KLST_TINY`

and for `SOFT`:

- `KLST_EMU`
- `KLST_STANDALONE` ==@todo== @NOTE(not sure about this one)

programatically identifiable e.g

```
#if defined(KLST_BOARD_KLST_PANDA)
#endif // KLST_BOARD_KLST_PANDA
```
