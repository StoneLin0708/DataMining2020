#include "combinations.h"
#include "data.h"
#include "gen.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using fmt::print;
using std::pair;
using std::string;
using std::vector;

struct candidates {
    struct VectorHasher {
        int operator()(const vector<int> &V) const {
            int hash = V.size();
            for (auto &i : V) {
                hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    std::unordered_map<vector<int>, int, VectorHasher> cmap;

    int k;

    candidates(const vector<vector<data::item>> &items, data &d,
               int min_support) {
        k = items[0].size();
        for (const auto &i : items)
            cmap[i] = 0;

        for (auto t : d.transactions()) {
            if (t.size() < k)
                continue;
            for_each_combination(t.begin(), t.begin() + k, t.end(),
                                 [&](auto b, auto e) {
                                     if (b == e)
                                         return false;
                                     auto q = std::vector<data::item>(b, e);
                                     auto c = cmap.find(q);
                                     if (c != cmap.end()) {
                                         ++(c->second);
                                     }
                                     return false;
                                 });
        }
        for (auto i = cmap.begin(); i != cmap.end();) {
            if (i->second < min_support) {
                i = cmap.erase(i);
            } else
                ++i;
        }
    }

    vector<vector<data::item>> gen_next() const {

        auto items = [this]() -> std::vector<data::item> {
            std::set<data::item> iset;
            for (const auto &c : cmap) {
                for (auto i : c.first) {
                    iset.insert(i);
                }
            }
            return {iset.begin(), iset.end()};
        }();

        vector<vector<data::item>> res;
        if (k + 1 > items.size())
            return {};
        for_each_combination(items.begin(), items.begin() + k + 1, items.end(),
                             [&](auto b, auto e) {
                                 if (b == e)
                                     return false;
                                 if (cmap.find({b, e - 1}) != cmap.end() ||
                                     cmap.find({b + 1, e}) != cmap.end()) {
                                     res.push_back({b, e});
                                 }
                                 return false;
                             });

        return res;
    }
};

vector<pair<vector<data::item>, int>> apriori(data &d, int min_support) {
    vector<pair<vector<data::item>, int>> result;
    vector<vector<data::item>> items;
    for (auto i : d.items())
        if (d.get_freq(i) >= min_support)
            items.push_back({i});
    for (int k = 1; !items.empty(); ++k) {
        candidates c(items, d, min_support);
        items = c.gen_next();
        for (auto i : c.cmap)
            result.push_back(i);
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 4)
        return 0;
    data d(argv[1]);
    int min_support = std::round(std::atof(argv[2]) * d.transactions().size());
    float confidence = std::atof(argv[3]);
    auto fp = apriori(d, min_support);
    std::sort(fp.begin(), fp.end(),
              [](auto a, auto b) { return a.second < b.second; });
    for (auto r : fp) {
        print("{}:", r.second);
        for (auto i : r.first)
            print("{},", d.to_name(i));
        print("\n");
    }

    auto rules = generate_rule(fp, confidence);
    std::sort(rules.begin(), rules.end(),
              [](auto a, auto b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto &rule : rules) {
        print("{} -> {} = {:.3} {:.3}\n", d.to_name(std::get<2>(rule)),
              d.to_name(std::get<3>(rule)), std::get<0>(rule),
              std::get<1>(rule) / float(d.num_trans()));
    }
    /*
    for (int i = d.num_items() - 1; i >= 0; --i)
        if (d.get_freq(i) >= min_support)
            print("{}:{}\n", d.get_freq(i), d.to_name(i));
    */
}