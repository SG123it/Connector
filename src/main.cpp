#include <iostream>
#include <filesystem>

#include <CFG_worker.hpp>
#include <Sync_logic.hpp>

int main(int argc, char* argv[]) {
    std::filesystem::path configuration_path = "-";
    bool configuration_found = false;
    if (argc > 1) {

        if(CFG_worker::CheckConfigurationFile(argv[1])) {
            configuration_path = argv[1];
            configuration_found = true;
        }
    }

    std::cout << "\n-----------------------------\n";
    std::cout << "Connector - A program for synchronizing folders" << std::endl;
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "-----------------------------\n\n\n";
    CFG_worker::Configuration_Data data;

    if (configuration_found) {
        data = CFG_worker::GetConfigurationData(configuration_path);
    }
    else {
        data = CFG_worker::InteractiveConfigurationCreating();
    }

    std::cout << "\n\n\n";
    std::cout << "========================================";
    std::cout << "\nSync FROM: " << data.path << std::endl;
    for (auto el : data.Child_objects) {
        std::cout << "TO: " << el << std::endl;
    }
    std::cout << "\n!!! | PLEASE MAKE SURE THAT NO ONE PROGRAM INTERFERES DURING SYNCHRONIZATION | !!!\n";
    std::cout << "!!! | PRESS ANY KEY TO CONTINUE... | !!!\n\n";
    std::cin.get();

    synchronization_logic::StartCopyProcess(data);
    
    std::cout << "\nPress any key to exit...\n";
    std::cin.get();


    return 0;
}