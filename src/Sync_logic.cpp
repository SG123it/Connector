#include "Sync_logic.hpp"

void synchronization_logic::StartCopyProcess(CFG_worker::Configuration_Data parent_CFG)
{
    std::unordered_map<std::string, bool> synchronization_status;

    std::cout << "Synchronization started..." << std::endl;
    std::cout << "\n========================================\n";

    for (auto Child_CFG : parent_CFG.Child_objects) {

        //[1/2] Синхронизация с дочерними объектами: копирование файлов
        for (auto& file : std::filesystem::recursive_directory_iterator(parent_CFG.path)) {
            if (file.path().filename() == CFG_worker::Standart_CFG_Name) continue;
            std::error_code error_code;

            try {
                //Child path + относительный путь до файла от parent path
                std::filesystem::path CopyTo = Child_CFG / std::filesystem::relative(file, parent_CFG.path);

                std::filesystem::copy(file, CopyTo, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive, error_code);
            } catch(std::filesystem::filesystem_error& err) {
                std::cout << "\nError: Unable to sync file: " << file.path().string() << std::endl;
                std::cout << "Err.what()" << err.what() << std::endl;
                std::cout << "std::error code variable: " << error_code.value() << std::endl;

                continue;
            }
        }

        //[2/2] Синхронизация с дочерними объектами: удаление файлов / папок
        //Проверка файла в дочерней директории на наличие в родительской:
        //Если файл существует: всё ок
        //Если не существует - значит был удалён: надо удалить
        for (auto& file : std::filesystem::recursive_directory_iterator(Child_CFG)) {
            if (file.path().filename() == CFG_worker::Standart_CFG_Name) continue;
            std::error_code error_code;

            if (!std::filesystem::exists(parent_CFG.path / std::filesystem::relative(file, Child_CFG))) {

                try {
                    std::filesystem::remove(file);
                    std::cout << "\nRemoved: "  << file.path().string() << std::endl;
                } catch(std::filesystem::filesystem_error& err) {
                    std::cout << "\nError: Unable to remove: " << file.path().string() << std::endl;
                    std::cout << "Err.what()" << err.what() << std::endl;
                    std::cout << "std::error code variable: " << error_code.value() << std::endl;

                    continue;
                }
            }
        }
        
        std::cout << "FINISHED: " << Child_CFG << std::endl;

    }

    std::cout << "Synchronization finished!" << std::endl;
    std::cout << "========================================\n";

}