#pragma once

#include <iostream>

#include <CFG_worker.hpp>

class synchronization_logic {
public:
    static void StartCopyProcess(CFG_worker::Configuration_Data parent_CFG);

private:
    static bool firststep(CFG_worker::Configuration_Data parent_CFG, std::filesystem::path Child_CFG_path);
    static bool secondstep(CFG_worker::Configuration_Data parent_CFG, std::filesystem::path Child_CFG_path);

    static std::vector<std::filesystem::path> GetDirectoryFiles(std::filesystem::path path);
};