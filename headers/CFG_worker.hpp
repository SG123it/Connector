#include <string>
#include <filesystem>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

class CFG_worker {
public:
    struct Configuration_Data {
        std::string type = "";
        std::string path = "";

        std::vector<std::string> Child_objects;
        std::unordered_map<std::filesystem::path, long long int> DateOfLastChange;
    };
    
    //Получить данные из конфигурационного файла
    static Configuration_Data GetConfigurationData(std::filesystem::path path);

    //Запуск интерактивного создания конфигурации
    static void InteractiveConfigurationCreating();

    //Проверить конфигурационный файл на соответствие стандарту
    static bool CheckConfigurationFile(std::filesystem::path path);

private:
    inline static const std::string CFG_Name = "ConnectorConfig.jccf";

    //Записать конфигурацию в конфигурационный файл
    static bool WriteToCFGFile(Configuration_Data data);

    static std::string TransformToLower(std::string text);

};