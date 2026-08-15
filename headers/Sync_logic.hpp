#pragma once

#include <iostream>

#include <CFG_worker.hpp>

class synchronization_logic {
public:
    synchronization_logic(CFG_worker::Configuration_Data CFG);
private:
    bool childOBJ_sync(std::filesystem::path ChildPath, std::filesystem::path ParentPath);
};