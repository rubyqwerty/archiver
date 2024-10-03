#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "tuple"

class DataReader
{
public:
    DataReader(std::string path_to_file) : file_(path_to_file), size_file(std::filesystem::file_size(std::filesystem::path(path_to_file))) {}

    std::vector<char> GetBlockData(int size)
    {
        std::vector<char> data(size);
        file_.read(&data[0], size);
        return data;
    }

    void InStart()
    {
        file_.seekg(0, std::ios::beg);
    }

    auto GetSizeFile() { return size_file; }

private:
    std::fstream file_;
    int size_file{0};
};