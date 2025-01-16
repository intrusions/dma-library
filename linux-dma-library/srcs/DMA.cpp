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

bool DMA::dma_init() noexcept
{
    logger.log(log_level::info, "Starting DMA initialization...");
    
    uint32_t ac = 3;
    const char *av[] = {
        av[0] = "",
        av[1] = "-device",
        av[2] = "fpga"
    };

    vmm_handle = VMMDLL_Initialize(ac, av);
    if (!vmm_handle) {
        logger.log(log_level::error, "DMA initialization failed: Unable to initialize VMMDLL");
        return false;
    }

    unsigned long long fpga_id = 0;
    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_FPGA_ID, &fpga_id)) {
        logger.log(log_level::warning, "Failed to retrieve FPGA ID (LC_OPT_FPGA_FPGA_ID)");
    }

    unsigned long long device_id = 0;
    if (!VMMDLL_ConfigGet(vmm_handle, LC_OPT_FPGA_DEVICE_ID, &device_id)) {
        logger.log(log_level::warning, "Failed to retrieve Device ID (LC_OPT_FPGA_DEVICE_ID)");
    }

    fpga = FPGA(fpga_id, device_id);
    logger.log(log_level::info, "DMA initialization completed successfully");
    logger.log(log_level::info, "FPGA ID: [{}], Device ID: [{}]", fpga._fpga_id, fpga._device_id);
    return true;
}

bool DMA::process_init(const char *name) noexcept
{
    logger.log(log_level::info, "Finding {} PID...", name);

    uint32_t pid;
    if (!VMMDLL_PidGetFromName(vmm_handle, name, &pid)) {
        logger.log(log_level::error, "Failed to retrieve {} PID", name);
        return false;
    }

    logger.log(log_level::info, "PID retrieved successfully");
    logger.log(log_level::info, "PID: [{}]", pid);

    logger.log(log_level::info, "Finding {} base address...", name);

    PVMMDLL_MAP_MODULEENTRY module_entry;
    if (!VMMDLL_Map_GetModuleFromNameU(
                            vmm_handle,
                            pid,
                            name,
                            &module_entry,
                            VMMDLL_MODULE_FLAG_NORMAL)) {
        logger.log(log_level::error, "Failed to retrieve {} base address", name);
        return false;
    }

    logger.log(log_level::info, "Base address retrieved successfully");
    logger.log(log_level::info, "Base address: [{:#018x}], base size: [{:#x}]", module_entry->vaBase, module_entry->cbImageSize);

    process = Process(
                pid,
                name,
                module_entry->vaBase,
                module_entry->cbImageSize);
    
    return true;
}

bool DMA::write_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept
{
    if (!VMMDLL_MemWrite(
                vmm_handle,
                process._pid,
                addr,
                static_cast<uint8_t*>(buff),
                size)) {
        logger.log(log_level::warning, "Failed to write [{}] to [{:#018x}]", buff, addr);
        return false;
    }
        
    return true;
}

bool DMA::read_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept
{
    if (!VMMDLL_MemRead(
                vmm_handle,
                process._pid,
                addr,
                static_cast<uint8_t*>(buff),
                size)) {
        logger.log(log_level::warning, "Failed to read [{}] at [{:#018x}]", buff, addr);
        return false;
    }

    return true;
}
