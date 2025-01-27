# dma-library

*A lightweight DMA library designed for game hacking. Built on Linux, targeting Windows 26100.2605+.*  
*The project will be updated according to my needs.*

---

## Features
- Retrieve PID(s) of processes by name.
- Read and write directly to the memory of target processes.
- Retrieve module base addresses in the target process.
- Track key presses and state changes.

---

## Dependencies

You need to previously build the following shared libraries:  
`leechcore.so`, `vmm.so`, and `leechcore_ft601_driver_linux.so`.  

Place them in `dma-library/libs/binary` as shown below:

```bash
.
├── dma-library
│   ├── includes
│   │   ├── DMA.hpp
│   │   └── Logger.hpp
│   ├── libs
│   │   ├── binary
│   │   │   ├── leechcore_ft601_driver_linux.so
│   │   │   ├── leechcore.so
│   │   │   └── vmm.so
│   │   └── includes
│   │       ├── leechcore.h
│   │       └── vmmdll.h
│   ├── Makefile
│   ├── README.md
│   └── srcs
│       └── DMA.cpp
└── example
    ├── main.cpp
    └── Makefile
```

- https://github.com/ufrisk/LeechCore  
- https://github.com/ufrisk/MemProcFS  
- https://github.com/ufrisk/LeechCore-plugins/tree/4c11afc5b68cc4b5e8e493788ab48d4f27fce954/leechcore_ft601_driver_linux

---

## Build

```bash
#! compile library
git clone https://github.com/intrusions/dma-library
cd ./dma-library/dma-library/
make

#! then, compile the example
cd ../example
make
```

You can replace the `/example/` directory with your custom cheats by adapting the Makefile.

---

## Usage

A minimalist example can be found in `/example/`.  
For a more complex use case in the context of game hacking, visit: [strafe](https://github.com/intrusions/strafe).

---

Thanks to @ufrisk for the awesome libraries and @Metick for the inspiration.
