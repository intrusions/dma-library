#include <iostream>

#include "DMA.hpp"
#include "Logger.hpp"

int main()
{
    DMA dma;

    if (!dma.dma_init()
            || !dma.keyboard_init()
            || !dma.process_init("writted.exe"))
        return EXIT_FAILURE;
    
    bool is_enable = false;

    for (;;) {
        if (dma.is_key_pressed(0x41)) {
            is_enable = !is_enable;

            if (is_enable) {
                std::cout << "enable" << std::endl;
            } else {
                std::cout << "disable" << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
