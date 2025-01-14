#pragma once

#include <cstdint>
#include <iostream>

#include "leechcore.h"
#include "vmmdll.h"

class Memory
{
private:
    VMM_HANDLE vmm_handle;

public:
    Memory();
    virtual ~Memory();

    bool dma_init();

};
