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

data::data(std::string path) : item_count(0) {
    std::ifstream data(path);
    std::vector<int> freq;
    std::string s;
    // read data
    while (std::getline(data, s)) {
        auto sp = split(s, ',');
        int N = sp.size();
        std::vector<item> items;
        for (int i = 1; i < N; ++i) { // for each item
            const auto sitem = name(sp[i]);
            if (name2item.find(sitem) == name2item.end()) { // create name map for new item
                freq.push_back(0);
                name2item[sitem] = static_cast<item>(item_count);
                ++item_count;
            }
            const auto id = name2item[sitem]; // map item string to item id
            items.push_back(id);
            ++freq[static_cast<int>(id)]; // count item frequency
        }
        _transactions.push_back(items);
    }
    this->trans_count = _transactions.size();

    // sort item by frequency
    std::vector<std::pair<int, item>> sorted;
    std::transform(freq.begin(), freq.end(), std::back_inserter(sorted),
                   [i = 0](auto f) mutable { return std::make_pair(f, i++); });
    std::sort(sorted.begin(), sorted.end(),
              [](auto a, auto b) { return a.first > b.first; });
    // store sorted frequency
    std::transform(sorted.begin(), sorted.end(), std::back_inserter(this->freq),
                   [](auto i) { return i.first; });
    // create map from unsorted id to new sorted id
    std::unordered_map<item, item> map2new; 
    for (int i = 0; i < item_count; ++i) {
        map2new[sorted[i].second] = i;
    }
    // map all item id to new sorted id
    for (auto &t : _transactions) {
        for (auto &i : t)
            i = map2new.at(i);
        std::sort(t.begin(), t.end());
    }
    // update map id and create reverse map (item id to item string)
    for (const auto &i : name2item) {
        auto n = map2new[i.second];
        name2item[i.first] = n;
        _items.push_back(i.second);
        item2name[n] = i.first;
    }
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