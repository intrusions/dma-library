#include "DMA.hpp"
#include "Logger.hpp"

Logger logger;

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

    process = Process(
                pid,
                name,
                module_entry->vaBase,
                module_entry->cbImageSize);
    
    logger.log(log_level::info, "Base address: [{:#018x}], base size: [{:#x}]", module_entry->vaBase, module_entry->cbImageSize);
    logger.log(log_level::info, "Process initialization completed successfully");

    return true;
}

bool DMA::keyboard_init() noexcept
{
    logger.log(log_level::info, "Starting keyboard initialization...");

    uint32_t winlogon_pid = 0;
    if (!get_pid_from_name("winlogon.exe", &winlogon_pid)) {
        return false;
    }

    std::vector<uint32_t> pids;
    if (!get_all_pid_from_name("csrss.exe", pids)) {
        return false;
    }

    logger.log(log_level::info, "Finding win32k.sys base address...");
    uintptr_t module_base = 0;
    for (const auto &pid : pids) {
        logger.log(log_level::info, "Analyzing PID: [{}]", pid);

        module_base = VMMDLL_ProcessGetModuleBaseU(
                                            vmm_handle,
                                            pid,
                                            "win32k.sys");
        if (!module_base) {
            logger.log(log_level::info, "win32k.sys base address not found for PID: [{}]", pid);
            continue ;
        }
        logger.log(log_level::info, "win32k.sys base address: [{:#018x}]", module_base);

        uintptr_t g_session_global_slots = module_base + 0x82538;
        uintptr_t user_session_state = 0;

        for (uint8_t i = 0; i < 4; i++) {
            
            uintptr_t addr1 = 0;
            if (!read_process_memory(g_session_global_slots, &addr1, sizeof(addr1), pid)) {
                return false;
            }

            addr1 += (8 * i);

            uintptr_t addr2 = 0;
            if (!read_process_memory(addr1, &addr2, sizeof(addr2), pid)) {
                return false;
            }

            if (!read_process_memory(addr2, &user_session_state, sizeof(user_session_state), pid)) {
                return false;
            }

            if (user_session_state > 0x7FFFFFFFFFFF) {
                break ;
            }
        }

        uintptr_t gafasync_key_state_export = user_session_state + 0x3830;
        if (gafasync_key_state_export > 0x7FFFFFFFFFFF) {
            logger.log(log_level::info, "Keyboard initialization completed successfully");

            keyboard = Keyboard(
                winlogon_pid,
                gafasync_key_state_export,
                module_base
            );

            return true;
        }
    }

    logger.log(log_level::error, "Keyboard initialization failed: no valid gafasync_key_state_export found");
    return false;
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

bool DMA::write_process_memory(uintptr_t addr, void *buff, uint32_t size, uint32_t pid) const noexcept
{
    if (!VMMDLL_MemWrite(
                    vmm_handle,
                    pid,
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
        logger.log(log_level::warning, "Failed to read at [{:#018x}]", addr);
        return false;
    }

    return true;
}

bool DMA::read_process_memory(uintptr_t addr, void *buff, uint32_t size, uint32_t pid) const noexcept
{
    if (!VMMDLL_MemRead(
                    vmm_handle,
                    pid,
                    addr,
                    static_cast<uint8_t*>(buff),
                    size)) {
        logger.log(log_level::warning, "Failed to read at [{:#018x}]", addr);
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

bool DMA::get_all_pid_from_name(const char *name, std::vector<uint32_t> &pids) const noexcept
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

bool DMA::get_module_base_address(const char *module_name) noexcept
{
    logger.log(log_level::info, "Finding {} base address...", module_name);
    
    PVMMDLL_MAP_MODULEENTRY module_entry;
    if (!VMMDLL_Map_GetModuleFromNameU(
                                vmm_handle,
                                process._pid,
                                module_name,
                                &module_entry,
                                VMMDLL_MODULE_FLAG_NORMAL)) {
        logger.log(log_level::error, "Failed to retrieve {} base address", module_name);
        return false;
    }

    process._modules_base_address[std::string(module_name)] = module_entry->vaBase;
    logger.log(log_level::info, "{} base address: [{:#018x}]", module_name, process._modules_base_address[module_name]);

    return true;
}

bool DMA::update_keys() noexcept
{
    keyboard._state_bitmap_update = std::chrono::system_clock::now();
    uint8_t previous_key_state_bitmap[64] = {0};
    memcpy(previous_key_state_bitmap, keyboard._state_bitmap, 64);

    if (!VMMDLL_MemReadEx(
                    vmm_handle,
                    keyboard._winlogon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY,
                    keyboard._gafasync_key_state_export,
                    reinterpret_cast<uint8_t*>(&keyboard._state_bitmap),
                    64,
                    NULL,
                    VMMDLL_FLAG_NOCACHE)) {
        logger.log(log_level::error, "Failed to read at [{:#018x}]", keyboard._gafasync_key_state_export);
        return false;
    }
    
    for (int vk = 0; vk < 256; ++vk) {
        if ((keyboard._state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2)
                && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2)) {
            keyboard._previous_state_bitmap[vk / 8] |= 1 << vk % 8;
        }
    }

    return true;
}

bool DMA::is_key_down(uint32_t key_code) noexcept
{
    if (keyboard._gafasync_key_state_export < 0x7FFFFFFFFFFF) {
        logger.log(log_level::error, "Keyboard unitialized");
        return false;
    }

    if (std::chrono::system_clock::now() - keyboard._state_bitmap_update > std::chrono::milliseconds(100)) {
        update_keys();
    }

    return (keyboard._state_bitmap[(key_code * 2 / 8)] & 1 << key_code % 4 * 2);
}

bool DMA::is_key_pressed(uint32_t key_code) noexcept
{
    static std::unordered_map<uint32_t, bool> previous_states;

    if (keyboard._gafasync_key_state_export < 0x7FFFFFFFFFFF) {
        logger.log(log_level::error, "Keyboard uninitialized");
        return false;
    }

    if (std::chrono::system_clock::now() - keyboard._state_bitmap_update > std::chrono::milliseconds(100)) {
        update_keys();
    }

    bool current_state = (keyboard._state_bitmap[(key_code * 2 / 8)] & 1 << key_code % 4 * 2);

    if (current_state && !previous_states[key_code]) {
        previous_states[key_code] = true;
        return true;
    }

    if (!current_state) {
        previous_states[key_code] = false;
    }

    return false;
}

const DMA::Process DMA::get_process() noexcept
{
    return process;
}
