#include "Sync_logic.hpp"

void synchronization_logic::StartCopyProcess(CFG_worker::Configuration_Data parent_CFG)
{
    std::unordered_map<std::string, bool> synchronization_status;

    std::cout << "\n========================================\n";
    std::cout << "Synchronization started..." << std::endl;

    for (auto Child_CFG : parent_CFG.Child_objects) {

        for (auto& file : std::filesystem::recursive_directory_iterator(parent_CFG.path)) {
            if (file.path().filename() == CFG_worker::Standart_CFG_Name) continue;

            std::filesystem::path CopyTo = Child_CFG / std::filesystem::relative(file, parent_CFG.path);

            std::filesystem::copy(file, CopyTo, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive);
        }

        for (auto& file : std::filesystem::recursive_directory_iterator(Child_CFG)) {
            if (file.path().filename() == CFG_worker::Standart_CFG_Name) continue;

            std::filesystem::path CheckExistFile = parent_CFG.path / std::filesystem::relative(file, Child_CFG);

            if (!std::filesystem::exists(CheckExistFile)) {
                std::filesystem::remove(file);
            }
        }
        
        std::cout << "FINISHED: " << Child_CFG << std::endl;

    }

    std::cout << "Synchronization finished!" << std::endl;
    std::cout << "========================================\n";

}