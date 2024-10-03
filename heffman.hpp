#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <bitset>
#include <set>
#include <iterator>
#include <queue>
#include <map>

template <class T>
struct node
{
    int weight;
    T value;
    std::shared_ptr<node<T>> left{nullptr};
    std::shared_ptr<node<T>> right{nullptr};

    node(T _value, int _weight) : value(_value), weight(_weight){};
    node(T _value, int _weight, std::shared_ptr<node<T>> l, std::shared_ptr<node<T>> r) : value(_value), weight(_weight), left(l), right(r)
    {
    }
    bool operator<(const node<T> &value) const
    {
        return weight > value.weight;
    }
    bool operator==(const node<T> &value) const
    {
        return weight == value.weight;
    }
    node(const node<T> &nd)
    {
        weight = nd.weight;
        value = nd.value;
        left = std::move(nd.left);
        right = std::move(nd.right);
    }
    node<T> &operator=(const node<T> &nd)
    {
        weight = nd.weight;
        value = nd.value;
        left = std::move(nd.left);
        right = std::move(nd.right);
        return *this;
    }
};

class heffman
{
public:
    heffman() {}
    heffman(std::map<char, int> chars)
    {
        for (auto &[first, count] : chars)
        {
            // std::cout << first << " " << count << "\n";
            nodes.push({first, count});
        }
    }

    std::map<char, int> &&GetCodesLatter()
    {
        auto tree{GetTree()};
        set_code<char>(tree);
        return std::move(codes);
    }

    std::map<char, int> &&GetLenghtCodes()
    {
        return std::move(length_code);
    }

    std::shared_ptr<node<char>> GetTree()
    {
        while (nodes.size() != 1)
        {
            auto first = nodes.top();
            nodes.pop();
            auto second = nodes.top();
            nodes.pop();
            node<char> new_elem(' ', first.weight + second.weight, std::make_shared<node<char>>(first), std::make_shared<node<char>>(second));
            nodes.push(new_elem);
        }

        return std::make_shared<node<char>>(nodes.top());
    }

private:
    template <class T>
    void set_code(const std::shared_ptr<node<T>> &root, int code = 0, int deep = 0)
    {
        if (!root->left && !root->right)
        {
            codes.insert({root->value, code});
            length_code.insert({root->value, deep});
        }
        if (root->left)
            set_code(root->left, (code << 1) | 1, deep + 1);
        if (root->right)
            set_code(root->right, (code << 1), deep + 1);
    }

    std::priority_queue<node<char>> nodes{};
    std::map<char, int> codes{};
    std::map<char, int> length_code{};
};