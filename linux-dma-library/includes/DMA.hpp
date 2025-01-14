#pragma once

#include <cstdint>
#include <iostream>

#include "leechcore.h"
#include "vmmdll.h"

class DMA
{
private:
    struct FPGA
    {
        uint64_t _fpga_id;
        uint64_t _device_id;

        FPGA(uint64_t process_id = 0, uint64_t device_id = 0)
            : _fpga_id(process_id)
            , _device_id(device_id) {}
    };
    struct Process
    {
        uint32_t    _pid;
        std::string _name;

        Process(uint64_t pid = 0, std::string name = "")
            : _pid(pid)
            , _name(name) {}
    };

    FPGA fpga { };
    Process process { };

    VMM_HANDLE vmm_handle;

public:
    DMA();
    virtual ~DMA();

    bool dma_init();
    bool process_init(const char *name);

};
