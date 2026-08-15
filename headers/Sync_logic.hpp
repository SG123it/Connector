#pragma once

#include <iostream>

#include <CFG_worker.hpp>

class synchronization_logic {
public:
    static void StartCopyProcess(CFG_worker::Configuration_Data parent_CFG);
};