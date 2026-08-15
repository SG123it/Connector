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

    std::cout << "========================================";

    std::cout << "\n!!! | PLEASE MAKE SURE THAT NO ONE PROGRAM INTERFERES DURING SYNCHRONIZATION | !!!\n";
    std::cout << "!!! | PRESS ANY KEY TO CONTINUE... | !!!\n";

    std::cout << "========================================\n\n";

    std::cin.get();

    synchronization_logic::StartCopyProcess(data);


    return 0;
}