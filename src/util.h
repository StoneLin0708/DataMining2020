#include "data.h"
#include <cmath>
#include <fmt/format.h>

struct problem {
    data dataset;
    int min_support;
    float confidence;
};

problem parse(int argc, char *argv[]) {
    data d(argv[1]);
    float ms = std::atof(argv[2]);
    int msi;
    if (ms > 0 && ms < 1) {
        msi = std::round(ms * d.transactions().size());
    } else {
        msi = ms;
    }
    return {d, msi, static_cast<float>(std::atof(argv[3]))};
}

void dump(const std::vector<std::pair<std::vector<int>, int>> &fp,
          const std::vector<std::tuple<float, int, std::vector<int>,
                                       std::vector<int>>> &rules,
          const problem &p) {
    using fmt::print;
    for (auto r : fp) {
        print("{}:", r.second);
        for (auto i : r.first)
            print("{},", p.dataset.to_name(i));
        print("\n");
    }

    for (const auto &rule : rules) {
        print("{} -> {} = {:.3} {:.3}\n", p.dataset.to_name(std::get<2>(rule)),
              p.dataset.to_name(std::get<3>(rule)), std::get<0>(rule),
              std::get<1>(rule) / float(p.dataset.num_trans()));
    }
}