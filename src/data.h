#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

class data {
  public:
    using item = int;
    using name = std::string;

    data(std::string path);

    name to_name(item i) const { return item2name.find(i)->second; }
    item to_item(name n) const { return name2item.find(n)->second; }
    int get_freq(item i) const { return freq[static_cast<int>(i)]; }

    std::vector<name> to_name(std::vector<item> i) const {
        std::vector<name> r;
        std::transform(i.begin(), i.end(), std::back_inserter(r),
                       [this](auto x) { return to_name(x); });
        return r;
    }

    const std::vector<item> &items() { return _items; }

    const std::vector<std::vector<item>> transactions() {
        return _transactions;
    };

    int num_items() const { return item_count; }
    int num_trans() const { return trans_count; }

    void dump();

  private:
    int item_count;
    int trans_count;

    std::vector<item> _items;
    std::vector<std::vector<item>> _transactions;
    std::unordered_map<name, item> name2item;
    std::unordered_map<item, name> item2name;
    std::vector<int> freq;
};