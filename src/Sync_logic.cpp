#include "Sync_logic.hpp"

void synchronization_logic::StartCopyProcess(CFG_worker::Configuration_Data parent_CFG)
{
    std::unordered_map<std::string, bool> synchronization_status;

    std::cout << "\nSynchronization started...";

    for (auto el : parent_CFG.Child_objects) {
        const std::filesystem::path FullChildCFGPath = parent_CFG.GetFullCFGPath(el);

        if (!CFG_worker::CheckConfigurationFile(FullChildCFGPath)) throw std::runtime_error("ERROR! Configuration check error");
        std::cout << "\n\nChild object: " << FullChildCFGPath;

        CFG_worker::Configuration_Data Child_CFG = CFG_worker::GetConfigurationData(FullChildCFGPath);

        for (auto& file : std::filesystem::recursive_directory_iterator(parent_CFG.path)) {

            std::filesystem::path CopyTo = Child_CFG.path / std::filesystem::relative(file, parent_CFG.path);

            std::filesystem::copy(file, CopyTo, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive);
        }

        for (auto& file : std::filesystem::recursive_directory_iterator(Child_CFG.path)) {

            std::filesystem::path CheckExistFile = parent_CFG.path / std::filesystem::relative(file, Child_CFG.path);

            if (!std::filesystem::exists(CheckExistFile)) {
                std::filesystem::remove(file);
            }
        }
        

    }

}