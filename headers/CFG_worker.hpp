#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

class CFG_worker {
public:
    inline static const std::string Standart_CFG_Name = "ConnectorConfig.jccf";
    class Configuration_Data {
    public:
        std::filesystem::path GetFullCFGPath(std::filesystem::path path);

        std::string type = "";
        std::string path = "";

        std::vector<std::string> Child_objects;
        std::unordered_map<std::filesystem::path, long long int> DateOfLastChange;
    };
    
    //Получить данные из конфигурационного файла
    static Configuration_Data GetConfigurationData(std::filesystem::path path);

    //Запуск интерактивного создания конфигурации
    static Configuration_Data InteractiveConfigurationCreating();

    //Проверить конфигурационный файл на соответствие стандарту
    static bool CheckConfigurationFile(std::filesystem::path path);

private:

    //Записать конфигурацию в конфигурационный файл
    static bool WriteToCFGFile(Configuration_Data data);

    static std::string TransformToLower(std::string text);

};