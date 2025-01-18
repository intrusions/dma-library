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
    
    for (;;) {
        if (dma.is_key_down(0x1)) {
            std::cout << "pressed" << std::endl;
        } else {
            std::cout << "down" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
