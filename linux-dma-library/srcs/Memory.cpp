#include <iostream>

#include "Memory.hpp"
#include "Logger.hpp"

Memory::Memory()
{

}

Memory::~Memory()
{

}

bool Memory::initialization()
{
    logger.log(log_level::info, "DMA initialization started...");

    std::cout   
                << "VMMDLL_MemSize(NULL) call, "
                << "return is " << VMMDLL_MemSize(NULL)
                << std::endl;
    
    return true;
}
