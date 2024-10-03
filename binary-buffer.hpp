#include <bitset>
#include <vector>
#include <map>

const int size_buffer{102400000};

class BinaryBuffer
{
public:
    BinaryBuffer() : buffer(size_buffer) {}

    BinaryBuffer(std::vector<char> &&buf) : buffer(std::move(buf)) {}

    template <class T>
    void Add(T byte, int size_useful_information)
    {
        std::bitset<32> binary_byte(byte);
        for (int i = size_useful_information - 1; i >= 0; --i)
        {
            SetBit(current_occupancy, binary_byte[i]);
            current_occupancy++;
        }
    }

    char GetBit(int index)
    {
        char &byte = buffer[index / 8];
        std::bitset<8> binary_byte(byte);
        return binary_byte[8 - index % 8 - 1];
    }

    void WriteToFile(std::string path_to_file)
    {
        std::ofstream out(path_to_file);
        out.write(&buffer[0], current_occupancy / 8 + 1);
    }

private:
    void SetBit(int index, char bynary_symbol)
    {
        char &byte = buffer[index / 8];
        std::bitset<8> binary_byte(byte);
        binary_byte[8 - index % 8 - 1] = bynary_symbol;
        buffer[index / 8] = static_cast<char>(binary_byte.to_ulong());
    }

    std::vector<char> buffer;
    int current_occupancy{0};
};