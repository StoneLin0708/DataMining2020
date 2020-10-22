#include "combinations.h"
#include "data.h"
#include "fp_tree.h"
#include "gen.h"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <queue>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using fmt::print;
using std::string;

template <typename F> void dfs(fp_tree::node *root, F f) {
    std::stack<fp_tree::node *> s;
    s.push(root);
    while (!s.empty()) {
        fp_tree::node *curr = s.top();
        s.pop();
        f(curr);
        for (int i = curr->child.size() - 1; i >= 0; --i) {
            s.push(curr->child[i]);
        }
    }
}

struct patterntree {
    using node = fp_tree::node;
    node root;
    patterntree(int item) : root(item, nullptr){};

    void add_pattern(std::vector<data::item> p, int freq) {
        node *curr = &root;
        for (auto i : p) {
            node *child = curr->find(i);
            if (child == nullptr)
                child = curr->add_child(i);
            curr = child;
        }
        curr->freq += freq;
    }

    template <typename F> void solve(F f) {
        std::stack<node *> s;
        s.push(&root);

        while (!s.empty()) {
            node *curr = s.top();
            s.pop();
            if (curr->freq) {
                std::vector<int> pat;
                for (node *p_root = curr; p_root != &root;
                     p_root = p_root->parent) {
                    pat.push_back(p_root->item);
                }
                std::reverse(pat.begin(), pat.end());
                pat.push_back(root.item);
                f(pat, curr->freq);
            }
            for (int i = curr->child.size() - 1; i >= 0; --i) {
                s.push(curr->child[i]);
            }
        }
    }
};

std::vector<std::pair<std::vector<int>, int>> query(data &d, fp_tree &fpt,
                                                    int min_support) {
    const int N = fpt.link.size();
    std::vector<std::pair<std::vector<int>, int>> result;
    for (int q = 0; q < N; ++q) {
        // print("----------------\nquery for {}\n\n", d.item2name[q]);
        patterntree qtree(q);
        for (fp_tree::node *cond_i : fpt.link[q]) {
            std::vector<data::item> cond;
            for (auto *i = cond_i->parent; i != &fpt.root; i = i->parent)
                cond.push_back(i->item);
            std::reverse(cond.begin(), cond.end());
            for (int n = cond.size(); n > 0; --n) {
                for_each_combination(
                    cond.begin(), cond.begin() + n, cond.end(),
                    [&](auto b, auto e) {
                        if (b == e)
                            return false;
                        qtree.add_pattern({b, e}, cond_i->freq);
                        return false;
                    });
            }
        }

        qtree.solve([&](std::vector<int> n, int f) {
            if (f >= min_support)
                result.push_back({n, f});
        });
        result.push_back({{q}, d.get_freq(q)});
        // print("-------------\n", d.item2name[q]);
    }

    std::sort(result.begin(), result.end(),
              [](auto a, auto b) { return a.first > b.first; });
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 4)
        return 0;
    data d(argv[1]);
    fp_tree fpt(d);

    int min_support = std::round(std::atof(argv[2]) * d.transactions().size());
    float confidence = std::atof(argv[3]);

    if (false)
        dfs(&fpt.root, [&](fp_tree::node *n) {
            print("{}:{}\n",
                  static_cast<int>(n->item) >= 0
                      ? static_cast<std::string>(d.to_name(n->item))
                      : "null",
                  n->freq);
        });
    auto fp = query(d, fpt, min_support);
    for (auto r : fp) {
        if (r.second < min_support)
            continue;
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
        if (d.get_freq(i) >= 3)
            print("{}:{}\n", d.get_freq(i), d.to_name(i));
    */
}