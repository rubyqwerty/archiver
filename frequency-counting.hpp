#include <map>
#include <vector>

namespace frequency_counting
{
    void CalculateFrequency(std::map<char, int> &counter, std::vector<char> data)
    {
        for (auto &i : data)
        {
            if (counter.find(i) != std::end(counter))
                counter[i]++;
            else
                counter[i] = 1;
        }
    }
};