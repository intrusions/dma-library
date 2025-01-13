#include <iostream>

#include "Memory.hpp"

Memory::Memory()
{

}

Memory::~Memory()
{

}

bool Memory::initialization()
{
    std::cout << "testing call to pci-leech dynamic libs:" << std::endl;
    
    std::cout   
                << "VMMDLL_MemSize(NULL) call, "
                << "return is " << VMMDLL_MemSize(NULL)
                << std::endl;
    
    return true;
}
