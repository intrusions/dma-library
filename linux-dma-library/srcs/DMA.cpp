#include "DMA.hpp"
#include "Logger.hpp"

Logger logger;

DMA::DMA()
{

}

DMA::~DMA()
{
    if (vmm_handle) {
        VMMDLL_Close(vmm_handle);
    }
}

bool DMA::dma_init()
{
    logger.log(log_level::info, "Starting DMA initialization...");
    
    uint32_t ac = 3;
    const char *av[] = {
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

    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_FPGA_ID, reinterpret_cast<PULONG64>(&fpga._fpga_id))) {
        logger.log(log_level::warning, "Failed to retrieve FPGA ID (LC_OPT_FPGA_FPGA_ID)");
    }

    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_DEVICE_ID, reinterpret_cast<PULONG64>(&fpga._device_id))) {
        logger.log(log_level::warning, "Failed to retrieve Device ID (LC_OPT_FPGA_DEVICE_ID)");
    }

    logger.log(log_level::info, "DMA initialization completed successfully");
    logger.log(log_level::info, "FPGA ID: [{}], Device ID: [{}]", fpga._fpga_id, fpga._device_id);
    return true;
}

bool DMA::process_init(const char *name)
{
    logger.log(log_level::info, "Finding {} PID...", name);

    uint32_t pid;
    if (!VMMDLL_PidGetFromName(vmm_handle, name, &pid)) {
        logger.log(log_level::error, "Failed to retrieve {} PID", name);
        return false;
    }

    process = Process(pid, name);
    logger.log(log_level::info, "{} PID retrieved successfully: [{}]", process._name, process._pid);
    return true;
}
