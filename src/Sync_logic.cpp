#include "Sync_logic.hpp"

void synchronization_logic::StartCopyProcess(CFG_worker::Configuration_Data parent_CFG)
{
    std::unordered_map<std::string, bool> synchronization_status;

    std::cout << "Synchronization started..." << std::endl;

    for (auto Child_CFG : parent_CFG.Child_objects) {
        
        firststep(parent_CFG, Child_CFG);
        secondstep(parent_CFG, Child_CFG);

        
        std::cout << "FINISHED: " << Child_CFG << std::endl;

    }

    
    std::cout << "\n========================================\n";
    std::cout << "Synchronization finished!" << std::endl;

}

bool synchronization_logic::firststep(CFG_worker::Configuration_Data parent_CFG, std::filesystem::path Child_CFG_path)
{
    std::cout << "\n[1/2 step] Copy\n";

    //[1/2] Синхронизация с дочерними объектами: копирование файлов
    try {
        for (auto& file : GetDirectoryFiles(parent_CFG.path)) {
            if (file.filename() == CFG_worker::Standart_CFG_Name) continue;

            try {
                //Child path + относительный путь до файла от parent path
                std::filesystem::path CopyTo = Child_CFG_path / std::filesystem::relative(file, parent_CFG.path);

                std::filesystem::copy(file, CopyTo, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive);
            } catch(std::filesystem::filesystem_error& err) {
                std::cout << "\nError: Unable to sync file: " << file.string() << std::endl;
                std::cout << "Err.what(): " << err.what() << std::endl << std::endl;

                continue;
            }

            std::cout << "File synced: " << file << std::endl;
        }
    } catch(std::filesystem::filesystem_error& err) {
        std::cout << "\nUnknown sync error(step [1/2])" << std::endl;
        std::cout << "Err.what(): " << err.what() << std::endl;

        return false;
    }

    return true;

}

bool synchronization_logic::secondstep(CFG_worker::Configuration_Data parent_CFG, std::filesystem::path Child_CFG_path)
{
    std::cout << "\n[2/2 step] Remove\n";

    //[2/2] Синхронизация с дочерними объектами: удаление файлов / папок
    //Проверка файла в дочерней директории на наличие в родительской:
    //Если файл существует: всё ок
    //Если не существует - значит был удалён: надо удалить
    try {
        for (auto& file : GetDirectoryFiles(Child_CFG_path)) {
            if (file.filename() == CFG_worker::Standart_CFG_Name) continue;

            if (!std::filesystem::exists(parent_CFG.path / std::filesystem::relative(file, Child_CFG_path))) {

                try {
                    std::filesystem::remove(file);
                    std::cout << "\nRemoved: "  << file.string() << std::endl;
                } catch(std::filesystem::filesystem_error& err) {
                    std::cout << "Error: Unable to remove: " << file.string() << std::endl;
                    std::cout << "Err.what(): " << err.what() << std::endl;

                    continue;
                }
            }
        }
    } catch(std::filesystem::filesystem_error& err) {
        std::cout << "\nUnknown sync error(step [2/2])" << std::endl;
        std::cout << "Err.what(): " << err.what() << std::endl;

        return false;
    }

    return true;

}

std::vector<std::filesystem::path> synchronization_logic::GetDirectoryFiles(std::filesystem::path path)
{

    //Это специальная функция для получения всех файлов из директории(рекурсивно). Причина по которой реализация именно такая - ошибка когда рекуртивный итератор пытается прочитать такие папки как System Volume Information
    //Эта функция должна исправить эту ошибку.

    std::vector<std::filesystem::path> files;
    std::error_code error_code;

    auto iterator = std::filesystem::recursive_directory_iterator(
        path,
        std::filesystem::directory_options::skip_permission_denied,
        error_code
    );

    if (error_code) {
        return files;
    }

    auto end = std::filesystem::end(iterator);

    while (iterator != end) {
        files.push_back(iterator->path());
        
        error_code.clear();
        iterator.increment(error_code);

        if (error_code) {
            continue;
        }
    }

    return files;
}
