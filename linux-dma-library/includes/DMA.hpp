#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "leechcore.h"
#include "vmmdll.h"

#define N_TARGET_PID    1000

/*
* @class DMA
* @brief Provides functionality for interacting with processes and devices via DMA.
* 
* This class includes methods to initialize DMA, interact with processes, and manage keyboard state.
*/
class DMA
{
private:
    /*
    * @struct FPGA
    * @brief Represents the FPGA device information.
    */
    struct FPGA
    {
        uint64_t _fpga_id;
        uint64_t _device_id;

        FPGA(uint64_t process_id = 0, uint64_t device_id = 0)
            : _fpga_id(process_id)
            , _device_id(device_id) {}
    };

    /*
    * @struct Process
    * @brief Represents a process being managed via DMA.
    */
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

    /*
    * @struct Keyboard
    * @brief Represents the keyboard state and related information.
    */
    struct Keyboard
    {
        uint32_t _winlogon_pid;
        uint64_t _gafasync_key_state_export;
        uint64_t _win32k_base_address;
        uint8_t  _state_bitmap[64] { };
        uint8_t  _previous_state_bitmap[256 / 8] { };

        std::chrono::time_point<std::chrono::system_clock> _state_bitmap_update;

        Keyboard(uint32_t winlogon_pid = 0, uint64_t gafasync_key_state_export = 0, uint64_t win32k_base_address = 0)
            : _winlogon_pid(winlogon_pid)
            , _gafasync_key_state_export(gafasync_key_state_export)
            , _win32k_base_address(win32k_base_address)
            , _state_bitmap_update(std::chrono::system_clock::now()) {}

    };

    FPGA fpga { };          /// Instance of the FPGA structure
    Process process { };    /// Instance of the Process structure
    Keyboard keyboard { };  /// Instance of the Keyboard structure

    VMM_HANDLE vmm_handle;  /// Handle for interacting with the DMA library

public:
    DMA();
    virtual ~DMA();

    /*
    * @brief Initializes the DMA device.
    * @return True on success, false otherwise.
    */
    bool dma_init() noexcept;

    /*
    * @brief Initializes a process for interaction via DMA.
    * @param name Name of the process to initialize.
    * @return True on success, false otherwise.
    */
    bool process_init(const char *name) noexcept;

    /*
    * @brief Initializes keyboard state tracking.
    * @return True on success, false otherwise.
    */
    bool keyboard_init() noexcept;

    /*
    * @brief Writes data to a process's memory.
    * @param addr Address to write to.
    * @param buff Pointer to the buffer containing data to write.
    * @param size Size of the data to write.
    * @param pid Process ID to target. (Optional)
    * @return True on success, false otherwise.
    */
    bool write_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept;
    bool write_process_memory(uintptr_t addr, void *buff, uint32_t size, uint32_t pid) const noexcept;

    /*
    * @brief Reads data from a process's memory.
    * @param addr Address to read from.
    * @param buff Pointer to the buffer where the read data will be stored.
    * @param size Size of the data to read.
    * @Param pid Process ID to target. (Optional)
    * @return True on success, false otherwise.
    */
    bool read_process_memory(uintptr_t addr, void *buff, uint32_t size) const noexcept;
    bool read_process_memory(uintptr_t addr, void *buff, uint32_t size, uint32_t pid) const noexcept;

    /*
    * @brief Retrieves the PID of a process by its name.
    * @param name Name of the process.
    * @param pid Pointer to store the PID.
    * @return True on success, false otherwise.
    */
    bool get_pid_from_name(const char *name, uint32_t *pid) const noexcept;

    /*
    * @brief Retrieves all PIDs of processes matching a given name.
    * @param name Name of the processes to search for.
    * @param pids Vector to store the retrieved PIDs.
    * @return True on success, false otherwise.
    */
    bool get_all_pid_from_name(const char *name, std::vector<uint32_t> &pids) const noexcept;

    /*
    * @brief Updates the state of the keyboard.
    * @return True on success, false otherwise.
    */
    bool update_keys() noexcept;

    /*
    * @brief Checks if a specific key is currently pressed.
    * @param key_code Virtual key code of the key to check.
    * @return True if the key is pressed, false otherwise.
    * 
    * ; https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    *
    */
    bool is_key_down(uint32_t key_code) noexcept;

    /*
    * @brief Checks if a specific key has been pressed (like for enable/disable something).
    * @param key_code Virtual key code of the key to check.
    * @return True if the key is pressed, false otherwise.
    * 
    * ; https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    *
    */
    bool is_key_pressed(uint32_t key_code) noexcept;
};
