#include "Memory.hpp"
#include "Logger.hpp"

Logger logger;

Memory::Memory()
{

}

Memory::~Memory()
{

}

bool Memory::dma_init()
{
    logger.log(log_level::info, "Starting DMA initialization...");
    
    DWORD ac = 3;
    LPCSTR av[] = {
        const_cast<LPCSTR>(""),
        const_cast<LPCSTR>("-device"),
        const_cast<LPCSTR>("fpga://algo=0"),
        const_cast<LPCSTR>(""),
        const_cast<LPCSTR>(""),
        const_cast<LPCSTR>(""),
        const_cast<LPCSTR>("")
    };

    vmm_handle = VMMDLL_Initialize(ac, av);
    if (!vmm_handle) {
        logger.log(log_level::error, "DMA initialization failed: Unable to initialize VMMDLL");
        return false;
    }

    ULONG64 fpga_id = 0;
    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_FPGA_ID, &fpga_id)) {
        logger.log(log_level::warning, "Failed to retrieve FPGA ID (LC_OPT_FPGA_FPGA_ID)");
    }

    ULONG64 device_id = 0;
    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_DEVICE_ID, &device_id)) {
        logger.log(log_level::warning, "Failed to retrieve Device ID (LC_OPT_FPGA_DEVICE_ID)");
    }

    logger.log(log_level::info, "DMA initialization completed successfully. FPGA ID: [{}], Device ID: [{}]", fpga_id, device_id);
    return true;
}
