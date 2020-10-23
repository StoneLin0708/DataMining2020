#pragma once
#include "combinations.h"
#include "fp.h"
#include <fmt/format.h>
#include <tuple>
#include <vector>

auto generate_rule(std::vector<fp *> &fps, float confidence) {
    using fmt::print;
    using pattern = fp::pattern;
    std::vector<rule *> result;

    struct VecHasher {
        int operator()(const std::vector<int> &V) const {
            int hash = V.size();
            for (auto &i : V) {
                hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    std::unordered_map<pattern, int, VecHasher> fpmap;
    for (auto i : fps)
        fpmap[i->pat] = i->support;

    for (auto &fp : fps) {
        const int sp = fp->support;
        auto &pt = fp->pat;
        int N = pt.size();
        for (int k = 1; k < N; ++k) {
            for_each_combination(
                pt.begin(), pt.begin() + k, pt.end(), [&](auto b, auto e) {
                    std::vector<int> A(b, e);
                    const float conf = float(sp) / fpmap[A];
                    if (conf >= confidence) {
                        result.push_back(new rule{conf, sp, A, {e, pt.end()}});
                    }
                    return false;
                });
        }
    }

    std::sort(result.begin(), result.end(),
              [](auto a, auto b) { return a->confidence < b->confidence; });

    return result;
}