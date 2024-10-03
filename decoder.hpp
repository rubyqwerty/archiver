#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <unistd.h>

#include "zip.hpp"
class Decoder
{
public:
    Decoder(std::string path_to_file) : data_reader_(path_to_file){};

    void Decode()
    {
        auto meta{RestoreMetadata()};

        auto data{data_reader_.GetBlockData(std::get<1>(meta))};

        BinaryBuffer buf(std::move(data));
        heffman hfmn{std::get<0>(meta)};

        auto tree{hfmn.GetTree()};
        int index_buffer{0}, count_decode_symbols{0};
        while (count_decode_symbols < std::get<1>(meta))
        {
            decoding(tree, buf, index_buffer, count_decode_symbols);
        }

        std::ofstream out("unpacking");
        out.write(&decoding_data[0], decoding_data.length());
    }

private:
    std::tuple<std::map<char, int>, int> RestoreMetadata()
    {
        std::map<char, int> frequency_count{};
        int count_symbol_in_file{0};

        auto size_matadata{0};
        std::memcpy(&size_matadata, &data_reader_.GetBlockData(1)[0], 1);

        if (size_matadata == 0)
            size_matadata = 256;

        auto meta_data{data_reader_.GetBlockData(size_matadata * 5)};

        for (int i = 0, counter; counter < size_matadata; counter++)
        {
            char symbol;
            std::memcpy(&symbol, &meta_data[i], 1);
            i++;
            uint32_t coding_symbol;
            std::reverse(std::begin(meta_data) + i, std::begin(meta_data) + i + 4);
            std::memcpy(&coding_symbol, &meta_data[i], 4);
            i += 4;

            frequency_count.insert({symbol, coding_symbol});
            count_symbol_in_file += coding_symbol;
        }
        return {frequency_count, count_symbol_in_file};
    }

    void decoding(std::shared_ptr<node<char>> nd, BinaryBuffer &buf, int &index, int &counter)
    {
        if (!nd->left && !nd->right)
        {
            counter++;
            decoding_data += nd->value;
        }
        else if (buf.GetBit(index) == 1)
            decoding(nd->left, buf, ++index, counter);
        else
            decoding(nd->right, buf, ++index, counter);
    }

    DataReader data_reader_;
    std::string decoding_data{""};
};