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
    else {
        std::cout << "The file has been successfully found: " << path.string() << std::endl;
    }

    if(TransformToLower(path.extension().string()) != ".jccf") {
        std::cout << "Error: Extension != .jccf: " << path.string() << std::endl;
        return false;
    }
    else {
        std::cout << "Extension: .jccf: " << path.string() << std::endl;
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
    if (JsonData.count("DateOfLastChange") == 0 ) return false;



    if(JsonData["type"] == "Parent") {
        std::cout << "\nChild objects checking...";
        std::cout << "\n------------\n";
        for (int i = 0; i < JsonData["Child"].size(); i++) {
            std::string ChildItemPath = JsonData["Child"][std::to_string(i)]["path"];
            if (!CheckConfigurationFile(ChildItemPath)) {
                std::cout << "\nERROR! Configuration file of child item have an error!: " << ChildItemPath;
                return false;
            } 
        }

    }

    return true;
}

bool CFG_worker::WriteToCFGFile(Configuration_Data data)
{
    nlohmann::json JsonData;

    std::filesystem::path path = data.path;
    path /= Standart_CFG_Name;
    if (std::filesystem::exists(path)) {
        std::cout << "\nWARNING! CFG file already exist. Remove...\n";
        std::filesystem::remove(path);
    }
    
    std::ofstream File(path);
    if (!std::filesystem::exists(path.parent_path())) {
        std::cout << "Error: Unable to find path: " << path.string() << std::endl;
        return false;
    }
    if (!File.is_open()) {
        std::cout << "Error: Unable to open file: " << path.string() << std::endl;
        return false;
    }

    //-----------------------------

    if (data.type == "Parent") {
        std::vector<std::filesystem::path> paths;

        for (int i = 0; i < data.Child_objects.size(); i++) {
            JsonData["Child"][std::to_string(i)]["path"] = data.Child_objects[i];
        }

        for (auto& el : std::filesystem::recursive_directory_iterator(path.parent_path())) {
            paths.push_back(el.path());
        }
        for (int i = 0; i < paths.size(); i++) {
            if (paths[i].filename().string() == Standart_CFG_Name) continue;

            JsonData["DateOfLastChange"][std::to_string(i)]["path"] = paths[i];
            JsonData["DateOfLastChange"][std::to_string(i)]["date"] = static_cast<long long int>(std::filesystem::last_write_time(paths[i]).time_since_epoch().count());
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

        throw(std::runtime_error("An error occurred while scanning."));
    }

    std::ifstream FileRead(path);
    nlohmann::json JsonData;
    FileRead >> JsonData;
    FileRead.close();

    Return_data.type = JsonData["type"];
    std::cout << "Type detected: " << Return_data.type << std::endl;

    Return_data.path = JsonData["path"];
    std::cout << "Path detected: " << Return_data.path << std::endl;

    if (Return_data.type == "Parent") {
        for (int i = 0; i < JsonData["Child"].size(); i++) {
            Return_data.Child_objects.push_back(JsonData["Child"][std::to_string(i)]["path"]);
            std::cout << "New child object detected: " << JsonData["Child"][std::to_string(i)]["path"] << std::endl;
        }

        for (int i = 0; i < JsonData["DateOfLastChange"].size(); i++) {
            Return_data.DateOfLastChange[JsonData["DateOfLastChange"][std::to_string(i)]["path"]] = static_cast<long long int>(JsonData["DateOfLastChange"][std::to_string(i)]["date"]);
            std::cout << "New time point detected: " << static_cast<long long int>(JsonData["DateOfLastChange"][std::to_string(i)]["date"]) << "(" << JsonData["DateOfLastChange"][std::to_string(i)]["path"] << ")" << std::endl;
        }
    }

    return Return_data;
}

CFG_worker::Configuration_Data CFG_worker::InteractiveConfigurationCreating()
{
    int count_of_child_objects = 0;
    Configuration_Data Data;

    std::cout << "You have started creating a configuration file.";

    Data.type = "Parent";

    while(true) {
        std::string raw_path = "";

        std::cout << "\nPlease enter path to the folder: ";
        try {
            std::getline(std::cin, raw_path);
        } catch(...) {
            std::cout << "\n\nSomething went wrong! Restart...";
            continue;
        }

        if (!std::filesystem::exists(raw_path) || !std::filesystem::is_directory(raw_path)) continue;

        Data.path = raw_path;
        break;
    }

    while(true) {
        int count = 0;

        std::cout << "\nPlease enter count of child objects: ";
        try {
            std::cin >> count;
            std::cin.clear();
            std::cin.ignore();
        } catch(...) {
            std::cout << "\n\nSomething went wrong! Restart...";
            continue;
        }

        count_of_child_objects = count;
        break;
    }

    for (int i = 0; i < count_of_child_objects; i++) {
        std::string path = "";

        std::cout << "\nPlease enter path of #" << i + 1 << " child object folder\n>>> ";

        try {
            std::getline(std::cin, path);
        } catch(...) {
            std::cout << "\n\nSomething went wrong! Restart...";
            continue;
        }

        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) continue;

        Data.Child_objects.push_back(path);
    }

    WriteToCFGFile(Data);

    std::cout << "\nCreating configuration files of child objects...";
    for (auto el : Data.Child_objects) {
        Configuration_Data ChildObjects_CFG_Data;

        ChildObjects_CFG_Data.type = "Child";
        ChildObjects_CFG_Data.path = el;

        WriteToCFGFile(ChildObjects_CFG_Data);
    }

    return Data;
}

std::filesystem::path CFG_worker::Configuration_Data::GetFullCFGPath(std::filesystem::path path)
{
    std::filesystem::path return_value = path / Standart_CFG_Name;
    return return_value;
}
