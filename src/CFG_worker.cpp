#include "CFG_worker.hpp"
#include <iostream>
#include <format>
#include <limits>
#include <chrono>

bool CFG_worker::CheckConfigurationFile(std::filesystem::path path)
{
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path)) {
        std::cout << "Error: Unable to find path or path object is directory: " << path.string() << std::endl;
        return false;
    }

    if(TransformToLower(path.extension().string()) != ".jccf") {
        std::cout << "Error: Extension != .jccf: " << path.string() << std::endl;
        return false;
    }

    //--------------------------

    std::ifstream FileRead(path);
    nlohmann::json JsonData;
    if (!FileRead.is_open()) {
        std::cout << "Error: Unable to open file: " << path.string() << std::endl;
        return false;
    }

    FileRead >> JsonData;
    FileRead.close();

    if (!JsonData.contains("type") || !JsonData.contains("path")) return false;
    if(JsonData["type"] == "Parent") {
        if (JsonData.count("Child") == 0) {
            return false;
        }
    }

    std::cout << "\nConfiguration file has been successfully checked\n";
    return true;
}

bool CFG_worker::WriteToCFGFile(Configuration_Data data)
{
    nlohmann::json JsonData;

    std::filesystem::path path = data.path;
    path /= Standart_CFG_Name;
    if (std::filesystem::exists(path)) {
        std::cout << "WARNING! CFG file already exist. Remove..." << std::endl << std::endl;
        std::filesystem::remove(path);
    }
    
    std::ofstream File(path);
    if (!std::filesystem::exists(path.parent_path())) {
        std::cout << "Error: Unable to find path: " << path.string() << std::endl << std::endl;
        return false;
    }
    if (!File.is_open()) {
        std::cout << "Error: Unable to open file: " << path.string() << std::endl << std::endl;
        return false;
    }

    //-----------------------------

    if (data.type == "Parent") {
        std::vector<std::filesystem::path> paths;

        for (int i = 0; i < data.Child_objects.size(); i++) {
            JsonData["Child"][std::to_string(i)]["path"] = data.Child_objects[i];
        }

    }

    JsonData["type"] = data.type;
    JsonData["path"] = data.path;

    File << std::setw(4) << JsonData << std::endl;
    File.close();

    return true;
}

std::string CFG_worker::TransformToLower(std::string text)
{
    for(int i = 0; i < text.size(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') text[i] += 32;
    }

    return text;
}

CFG_worker::Configuration_Data CFG_worker::GetConfigurationData(std::filesystem::path path)
{
    Configuration_Data Return_data;
    if (!CheckConfigurationFile(path)) {
        if (std::filesystem::exists(path)) {
            std::cout << "File deleted: " << path << std::endl;
            std::filesystem::remove(path);
        }

        throw(std::runtime_error("An error occurred while scanning.\n\n"));
    }

    std::ifstream FileRead(path);
    nlohmann::json JsonData;
    FileRead >> JsonData;
    FileRead.close();

    Return_data.type = JsonData["type"];
    if (Return_data.type != "Parent" && Return_data.type != "Child") throw std::runtime_error("Error! Unknown type: " + Return_data.type);
    std::cout << "Type detected: " << Return_data.type << std::endl;

    Return_data.path = JsonData["path"];
    if (!std::filesystem::exists(Return_data.path) || !std::filesystem::is_directory(Return_data.path)) throw std::runtime_error("Error! path not found or path is file: " + Return_data.path);
    std::cout << "Path detected: " << Return_data.path << std::endl;

    if (Return_data.type == "Parent") {
        for (int i = 0; i < JsonData["Child"].size(); i++) {
            Return_data.Child_objects.push_back(JsonData["Child"][std::to_string(i)]["path"]);
            std::cout << "New child object detected: " << JsonData["Child"][std::to_string(i)]["path"] << std::endl;
        }
    }

    return Return_data;
}

CFG_worker::Configuration_Data CFG_worker::InteractiveConfigurationCreating()
{
    int count_of_child_objects = 0;
    Configuration_Data Data;

    std::cout << "You have started creating a configuration file." << std::endl;

    Data.type = "Parent";

    while(true) {
        std::string raw_path = "";

        std::cout << "Please enter path to the folder: " << std::endl;
        try {
            std::getline(std::cin, raw_path);
        } catch(...) {
            std::cout << "Something went wrong! Restart..." << std::endl << std::endl;
            continue;
        }

        if (!std::filesystem::exists(raw_path) || !std::filesystem::is_directory(raw_path)) continue;

        Data.path = raw_path;
        break;
    }

    while(true) {
        int count = 0;

        std::cout << "Please enter count of child objects: " << std::endl;
        try {
            std::cin >> count;
            std::cin.clear();
            std::cin.ignore();
        } catch(...) {
            std::cout << "Something went wrong! Restart..." << std::endl << std::endl;
            continue;
        }

        count_of_child_objects = count;
        break;
    }

    for (int i = 0; i < count_of_child_objects; i++) {
        std::string path = "";

        std::cout << "Please enter path of #" << i + 1 << " child object folder\n>>> " << std::endl;

        try {
            std::getline(std::cin, path);
        } catch(...) {
            std::cout << "Something went wrong! Restart..." << std::endl << std::endl;
            continue;
        }

        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) continue;

        Data.Child_objects.push_back(path);
    }

    WriteToCFGFile(Data);
    return Data;
}

std::filesystem::path CFG_worker::Configuration_Data::GetFullCFGPath(std::filesystem::path path)
{
    std::filesystem::path return_value = path / Standart_CFG_Name;
    return return_value;
}
