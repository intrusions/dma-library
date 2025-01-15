#include <iostream>

#include "DMA.hpp"
#include "Logger.hpp"

int main()
{
    DMA dma;

    if (!dma.dma_init()
            || !dma.process_init("CalculatorApp.exe"))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
