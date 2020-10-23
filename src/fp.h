#pragma once
#include <algorithm>
#include <vector>

struct fp {
    using pattern = std::vector<int>;
    pattern pat;
    int support;
};

struct rule {
    float confidence;
    int support;
    fp::pattern A;
    fp::pattern B;
};