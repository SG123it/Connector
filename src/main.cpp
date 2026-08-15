#include <iostream>
#include <filesystem>

#include <CFG_worker.hpp>
#include <Sync_logic.hpp>

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        std::cout << "Argument: " << argv[i] << std::endl;
    }

    std::cout << "\n-----------------------------\n";
    std::cout << "Connector - A program for synchronizing folders" << std::endl;
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "-----------------------------\n\n\n";

    return 0;
}