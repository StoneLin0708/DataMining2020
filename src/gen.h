#include "combinations.h"
#include <tuple>
#include <vector>

auto generate_rule(std::vector<std::pair<std::vector<int>, int>> &fps,
                   float confidence) {
    using pattern = std::vector<int>;
    std::vector<std::tuple<float, int, pattern, pattern>> result;

    struct VecHasher {
        int operator()(const std::vector<int> &V) const {
            int hash = V.size();
            for (auto &i : V) {
                hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    std::unordered_map<pattern, int, VecHasher> fpmap(fps.begin(), fps.end());

    for (auto &fp : fps) {
        float sp = fp.second;
        auto &pt = fp.first;
        int N = pt.size();
        for (int k = 1; k < N; ++k) {
            for_each_combination(
                pt.begin(), pt.begin() + k, pt.end(), [&](auto b, auto e) {
                    const std::vector<int> A(b, e);
                    const float conf = sp / fpmap[A];
                    if (conf >= confidence) {
                        result.push_back(
                            {conf, sp, std::vector<int>(e, pt.end()), A});
                    }
                    return false;
                });
        }
    }

    return result;
}