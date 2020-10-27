#include "combinations.h"
#include "data.h"
#include "fp.h"
#include "fp_tree.h"
#include "gen.h"
#include "util.h"
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

auto fpg(data &d, int min_support) {
    fp_tree fpt(d);
    const int N = fpt.link.size();
    std::vector<fp *> result;
    for (int q = 0; q < N; ++q) {
        if (d.get_freq(q) < min_support)
            continue;
        result.push_back(new fp{{q}, d.get_freq(q)});
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
            if (f >= min_support || !min_support)
                result.push_back(new fp{n, f});
        });
    }
    std::sort(result.begin(), result.end(),
              [](auto a, auto b) { return a->support < b->support; });
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 4)
        return 0;
    auto p = parse(argc, argv);
    auto fp = fpg(p.dataset, p.min_support);
    auto rules = generate_rule(fp, p.confidence);
    if(argc == 4) dump(fp, rules, p);
}