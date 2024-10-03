#include <filesystem>
#include "data-reader.hpp"
#include "frequency-counting.hpp"
#include "heffman.hpp"
#include "binary-buffer.hpp"

constexpr int sizeBlockToRead = 8;

struct serialiezer
{
    struct pair
    {
        uint8_t symbol;
        int count;
    };
    uint8_t count_codes;
    pair *pairs;
    uint8_t *zip;
};

class Coder
{
public:
    Coder(std::string file_name_to_zip) : file_to_zip(file_name_to_zip)
    {
        DoFrequencyCounting();
        EncodeSymbols();
    }

    auto GetZipData()
    {
        SetMetaData();

        if (file_to_zip.GetSizeFile() < sizeBlockToRead)
        {
            auto data{file_to_zip.GetBlockData(file_to_zip.GetSizeFile())};
            EncodeData(data);
        }
        else
        {
            auto count_iteration{file_to_zip.GetSizeFile() / sizeBlockToRead + 1};

            for (int i = 0; i < count_iteration; ++i)
            {
                auto rest{file_to_zip.GetSizeFile() - sizeBlockToRead * i};
                auto data{file_to_zip.GetBlockData(std::min(sizeBlockToRead, rest < 0 ? file_to_zip.GetSizeFile() : rest))};

                EncodeData(data);
            }
        }
        return std::move(buffer);
    }

private:
    void SetMetaData()
    {
        char count_codes = static_cast<char>(codes_.size());
        buffer.Add(count_codes, 8);
        for (auto &[symbol, code] : codes_)
        {
            buffer.Add(symbol, 8);
            buffer.Add(frequency_count[symbol], 32);
        }
    }

    void DoFrequencyCounting()
    {
        if (file_to_zip.GetSizeFile() < sizeBlockToRead)
        {
            auto data{file_to_zip.GetBlockData(file_to_zip.GetSizeFile())};
            frequency_counting::CalculateFrequency(frequency_count, data);
        }
        else
        {
            auto count_iteration{file_to_zip.GetSizeFile() / sizeBlockToRead + 1};

            for (int i = 0; i < count_iteration; ++i)
            {
                auto rest{file_to_zip.GetSizeFile() - sizeBlockToRead * i};
                auto data{file_to_zip.GetBlockData(std::min(sizeBlockToRead, rest < 0 ? file_to_zip.GetSizeFile() : rest))};

                frequency_counting::CalculateFrequency(frequency_count, data);
            }
        }

        file_to_zip.InStart();
    }

    void EncodeData(std::vector<char> &data)
    {
        for (auto i : data)
        {
            auto code{codes_[i]};
            auto length{length_code[i]};
            buffer.Add(code, length);
        }
    }

    void EncodeSymbols()
    {
        heffman hef(frequency_count);
        codes_ = hef.GetCodesLatter();
        length_code = hef.GetLenghtCodes();
    }

    BinaryBuffer buffer{};
    DataReader file_to_zip;

    std::map<char, int> frequency_count{};
    std::map<char, int> codes_;
    std::map<char, int> length_code{};
};