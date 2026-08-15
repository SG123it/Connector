#include <iostream>
#include <filesystem>

#include <CFG_worker.hpp>
#include <Sync_logic.hpp>

int main(int argc, char* argv[]) {
    std::filesystem::path configuration_path = "-";
    bool configuration_found = false;

    if (argc > 0) {

        if(CFG_worker::CheckConfigurationFile(argv[1])) {
            configuration_path = argv[1];
            configuration_found = true;
        }
        else return 1;

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


    return 0;
}