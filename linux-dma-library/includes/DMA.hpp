#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "leechcore.h"
#include "vmmdll.h"

#define N_TARGET_PID    1000

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
        uintptr_t   _base_address;
        uint32_t    _base_size;

        Process(uint32_t pid = 0, std::string name = "", uint64_t base_address = 0, uint32_t base_size = 0)
            : _pid(pid)
            , _name(name)
            , _base_address(base_address)
            , _base_size(base_size) {}
    };

    struct Keyboard
    {
        uint32_t _winlogon_pid;

        Keyboard(uint32_t winlogon_pid = 0)
            : _winlogon_pid(winlogon_pid) {}

    };

    FPGA fpga { };
    Process process { };
    Keyboard keyboard { };

    VMM_HANDLE vmm_handle;

public:
    DMA();
    virtual ~DMA();

    bool dma_init() noexcept;
    bool process_init(const char *name) noexcept;
    bool keyboard_init() noexcept;

    bool write_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept;
    bool read_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept;

    bool get_pid_from_name(const char *name, uint32_t *pid) const noexcept;
    bool get_all_pid_from_name(const char *name, std::vector<uint32_t>) const noexcept;

};
