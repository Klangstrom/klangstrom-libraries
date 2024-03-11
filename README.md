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

### architectures

available architectures are:

- SOFT/DESKTOP @TODO(need to decide on name)
- METAL/STM32 @TODO(need to decide on name)

programatically identifiable e.g

```
#if (KLST_ARCHITECTURE_METAL)
#endif // KLST_ARCHITECTURE_METAL
```

### boards

available boards are for `METAL`:

- `KLST_PANDA`
- `KLST_SHEEP`
- `KLST_TINY`

and for `SOFT`:

- `KLST_EMU`
- `KLST_STANDALONE` @NOTE(not sure about this one)

programatically identifiable e.g

```
#if defined(KLST_BOARD_KLST_PANDA)
#endif // KLST_BOARD_KLST_PANDA
```
