#include "data.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

data::data(std::string path) {
    std::ifstream data(path);
    std::string s;
    int item_count = 0;
    std::vector<int> freq;

    while (std::getline(data, s)) {
        auto sp = split(s, ',');
        int N = sp.size();
        std::vector<item> items;
        for (int i = 1; i < N; ++i) {
            const auto sitem = name(sp[i]);
            if (name2item.find(sitem) == name2item.end()) {
                freq.push_back(0);
                item2name[static_cast<item>(item_count)] = sitem;
                name2item[sitem] = static_cast<item>(item_count++);
            }
            const auto id = name2item[sitem];
            items.push_back(id);
            ++freq[static_cast<int>(id)];
        }
        _transactions.push_back(items);
    }

    std::vector<std::pair<int, item>> sorted;
    int item_num = freq.size();
    N = item_num;
    for (int i = 0; i < item_num; ++i)
        sorted.push_back(
            std::make_pair(freq[static_cast<int>(i)], static_cast<item>(i)));
    std::sort(sorted.begin(), sorted.end(),
              [](auto a, auto b) { return a.first > b.first; });
    for (auto i : sorted) {
        this->freq.push_back(i.first);
    }
    std::unordered_map<item, item> map2new(item_num);
    for (int i = 0; i < item_num; ++i) {
        map2new[sorted[i].second] = i;
    }

    for (auto &t : _transactions) {
        for (auto &i : t) {
            i = map2new[i];
        }
        std::sort(t.begin(), t.end());
    }

    std::unordered_map<item, name> s_i2n;

    for (auto &i : name2item) {
        auto n = map2new[i.second];
        name2item[i.first] = n;
        _items.push_back(i.second);
        s_i2n[n] = i.first;
    }
    item2name = s_i2n;
}

void data::dump() {
    for (auto t : _transactions) {
        for (auto i : t) {
            fmt::print("{}, ", item2name[i]);
        }
        fmt::print("\n");
    }
    for (auto i : item2name) {
        fmt::print("{}:{}\n", i.first, i.second);
    }
}