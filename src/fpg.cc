#include "combinations.h"
#include "data.h"
#include "fp_tree.h"
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

std::vector<std::pair<int, std::vector<int>>> query(data &d, fp_tree &fpt,
                                                    int min_support) {
    const int N = fpt.link.size();
    std::vector<std::pair<int, std::vector<int>>> result;
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
                result.push_back(std::make_pair(f, n));
        });
        // print("-------------\n", d.item2name[q]);
    }

    std::sort(result.begin(), result.end(),
              [](auto a, auto b) { return a.first < b.first; });
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        return 0;
    data d(argv[1]);
    print("build tree...\n");
    fp_tree fpt(d);
    print("done\n");

    int min_support = std::atof(argv[2]) * d.transactions().size();

    if (false)
        dfs(&fpt.root, [&](fp_tree::node *n) {
            print("{}:{}\n",
                  static_cast<int>(n->item) >= 0
                      ? static_cast<std::string>(d.to_name(n->item))
                      : "null",
                  n->freq);
        });
    auto res = query(d, fpt, min_support);
    for (auto r : res) {
        if (r.first < min_support)
            continue;
        print("{}:", r.first);
        for (auto i : r.second)
            print("{},", d.to_name(i));
        print("\n");
    }

    /*
    for (int i = d.num_items() - 1; i >= 0; --i)
        if (d.get_freq(i) >= 3)
            print("{}:{}\n", d.get_freq(i), d.to_name(i));
    */
}