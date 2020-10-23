#pragma once
#include "data.h"
#include "fp.h"
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

void dump(const std::vector<fp *> &fp, const std::vector<rule *> &rules,
          const problem &p) {
    using fmt::print;
    for (auto f : fp)
        print("{}:{}\n", f->support, p.dataset.to_name(f->pat));
    for (const auto &r : rules) {
        print("{} -> {} = {:.3} {:.3}\n", p.dataset.to_name(r->A),
              p.dataset.to_name(r->B), r->confidence,
              r->support / float(p.dataset.num_trans()));
    }
}