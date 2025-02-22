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

    while (true) {
        if (dma.is_key_pressed(0x41)) {
            is_enable = !is_enable;

            if (is_enable) {
                std::cout << "something enable" << std::endl;
            } else {
                std::cout << "something disable" << std::endl;
            }
        }
    }
}
