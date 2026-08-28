#include <iostream>
#include <filesystem>

#include <CFG_worker.hpp>
#include <Sync_logic.hpp>

int main(int argc, char* argv[]) {
    //Переменная для хранения пути до конфигурационного файла
    std::filesystem::path configuration_path = "-";

    //Переменная для хранения boolean-значения (существует ли конфигурационный файл)
    bool configuration_found = false;

    //Проверка есть ли аргументы(если argc == 1 их нет)
    if (argc > 1) {

        //Проверка существует ли конфигурационный файл. Если найден записывается путь и переменной configuration_found присваивается 1
        if(CFG_worker::CheckConfigurationFile(argv[1])) {
            configuration_path = argv[1];
            configuration_found = true;
        }
    }

    //Вывод информации о програме
    std::cout << "\n-----------------------------\n";
    std::cout << "Connector - A program for synchronizing folders" << std::endl;
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "-----------------------------\n\n\n";

    //Переменная с полями конфигурационного файла
    CFG_worker::Configuration_Data data;

    //Действие в зависимости от того, найден ли конфигурационный файл
    if (configuration_found) {
        //Если CFG найден - запуск функции для получение данных оттуда
        data = CFG_worker::GetConfigurationData(configuration_path);
    }
    else {
        //Иначе - запуск функции для интерактивного создания конфигурационного файла
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

    //Основная функция запускающая процесс синхронизации
    synchronization_logic::StartCopyProcess(data);
    
    std::cout << "\nPress any key to exit...\n";
    std::cin.get();


    return 0;
}