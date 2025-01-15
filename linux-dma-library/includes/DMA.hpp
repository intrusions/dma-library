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
        std::string _name;
        uint32_t    _pid;
        uint64_t    _base_address;
        uint32_t    _base_size;

        Process(uint32_t pid = 0, std::string name = "", uint64_t base_address = 0, uint32_t base_size = 0)
            : _pid(pid)
            , _name(name)
            , _base_address(base_address)
            , _base_size(base_size) {}
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
