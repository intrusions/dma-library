# dma-library

*A minimalist DMA library designed for game hacking. Built on Linux, targeting Windows 26100.2605.*  
*The project is not finished and will be updated depending of my needs.*

---

## Features
- Retrieve PID(s) of processes by name.
- Read and write directly to the memory of target processes.
- Retrieve module base addresses in the target process.
- Track key presses.

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
# compile library
git clone https://github.com/intrusions/dma-library
cd ./dma-library/dma-library/
make

# then, compile the example
cd ../example
make
```

---

## Usage

A minimalist example can be found in `/example/`.  
For a more complex use case in the context of game hacking, visit: [strafe](https://github.com/intrusions/strafe).

By default, the program provides detailed logs on DMA initialization and execution.
To disable logs, remove `-DDEBUG` from the `Makefile` before compiling.

Log example:
```
[16:02:34] [*] Starting DMA initialization...
[+] using FTDI device: 0403:601f (bus 2, device 3)
[+] FTDIFTDI SuperSpeed-FIFO Bridge000000000001
[16:02:37] [*] DMA initialization completed successfully
[16:02:37] [*] FPGA ID: [4], Device ID: [768]
[16:02:37] [*] Starting keyboard initialization...
[16:02:37] [*] Finding winlogon.exe PID...
[16:02:37] [*] winlogon.exe PID: [1996]
[16:02:37] [*] Finding csrss.exe PIDs...
[16:02:37] [*] 201 PIDs found on the target system
[16:02:37] [*] csrss.exe PID: [1728]
[16:02:37] [*] csrss.exe PID: [1820]
[16:02:37] [*] Finding win32k.sys base address...
[16:02:37] [*] Analyzing PID: [1728]
[16:02:38] [*] win32k.sys base address: [0xfffff8058c540000]
[16:02:38] [*] Keyboard initialization completed successfully
[16:02:38] [*] Starting process initialization...
[16:02:38] [*] Finding cs2.exe PID...
[16:02:38] [*] cs2.exe PID: [13528]
[16:02:38] [*] Finding cs2.exe base address...
[16:02:43] [*] Base address: [0x00007ff6a0130000], base size: [0x432000]
[16:02:43] [*] Process initialization completed successfully
[16:02:43] [*] Finding client.dll base address...
[16:02:43] [*] client.dll base address: [0x00007ffc879a0000]
[16:02:43] [*] Finding engine2.dll base address...
[16:02:43] [*] engine2.dll base address: [0x00007ffca4880000]
```

---

Thanks to @ufrisk and @Metick for the awesome libraries.

---

**Educational purpose only.**
