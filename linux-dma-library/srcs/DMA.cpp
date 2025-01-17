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
    logger.log(log_level::info, "Starting process initialization...");

    uint32_t pid;
    if (!get_pid_from_name(name, &pid)) {
        return false;
    }

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

bool DMA::keyboard_init() noexcept
{
    logger.log(log_level::info, "Starting keyboard initialization...");

    uint32_t logon_pid = 0;
    if (!get_pid_from_name("winlogon.exe", &logon_pid)) {
        return false;
    }

    std::vector<uint32_t> pids;
    if (!get_all_pid_from_name("csrss.exe", pids)) {
        return false;
    }

    // IN CONSTRUCTION

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

bool DMA::get_pid_from_name(const char *name, uint32_t *pid) const noexcept
{
    logger.log(log_level::info, "Finding {} PID...", name);

    if (!VMMDLL_PidGetFromName(
                        vmm_handle,
                        name,
                        pid)) {
        logger.log(log_level::error, "Failed to retrieve {} PID", name);
        return false;
    }
    
    logger.log(log_level::info, "{} PID: [{}]", name, *pid);
    return true;
}

bool DMA::get_all_pid_from_name(const char *name, std::vector<uint32_t> pids) const noexcept
{
    logger.log(log_level::info, "Finding {} PIDs...", name);

    uint64_t size = N_TARGET_PID;
    uint32_t pid[N_TARGET_PID];

    if (!VMMDLL_PidList(
                vmm_handle,
                pid,
                &size)) {
        logger.log(log_level::error, "Failed to retrieve PID list on the target system");
        return false;            
    }
    logger.log(log_level::info, "{} PIDs found on the target system", size);

    for (uint16_t i = 0; i < size; i++) {
        VMMDLL_PROCESS_INFORMATION process_info;
        memset(&process_info, 0, sizeof(VMMDLL_PROCESS_INFORMATION));
        
        process_info.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
        process_info.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;

        uint64_t struct_size = sizeof(VMMDLL_PROCESS_INFORMATION);

        if (!VMMDLL_ProcessGetInformation(
                                vmm_handle,
                                pid[i],
                                &process_info,
                                &struct_size)) {
            logger.log(log_level::warning, "Failed to retrieve process information [PID: {}]", pid[i]);
            continue;
        }

        if (!strcmp(process_info.szNameLong, name)) {
            logger.log(log_level::info, "{} PID: [{}]", name, process_info.dwPID);
            pids.push_back(process_info.dwPID);
        }
    }

    if (pids.empty()) {
        logger.log(log_level::info, "{} PIDs not found", name);
        return false;
    }

    return true;
}
