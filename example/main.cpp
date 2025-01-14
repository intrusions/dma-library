#include <iostream>

#include "Memory.hpp"
#include "Logger.hpp"

int main()
{
    Memory memory;
    memory.dma_init();

    return EXIT_SUCCESS;
}
