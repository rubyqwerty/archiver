#include <numeric>
#include <cstring>
#include <string>
#include "decoder.hpp"

int main(int argc, char **argv)
{

    bool is_pack = 1;
    if (argc < 2)
    {
        std::cout << "<path to file> <0 - unpacking | 1 - packing>\n";
        return 0;
    }

    std::string path_to_file{argv[1]};

    if (argc == 3)
        is_pack = atoi(argv[2]);

    if (is_pack)
    {
        Coder zip{path_to_file};
        auto zip_data{zip.GetZipData()};

        zip_data.WriteToFile(path_to_file + ".mzip");
    }
    else
    {
        Decoder decoder{path_to_file};
        decoder.Decode();
    }
}
