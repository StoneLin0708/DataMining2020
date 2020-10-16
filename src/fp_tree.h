#pragma once
#include "data.h"
#include <stack>
#include <string>

struct fp_tree {
    struct node {
        node(const data::item item, node *parent)
            : item(item), parent(parent) {}
        ~node() {
            for (auto i : child)
                delete i;
        }
        const data::item item;
        node *parent;
        int freq = 0;

        node *find(data::item item) const;

        node *add_child(data::item item);

        std::vector<node *> child;
    };

    node root{-1, nullptr};

    std::vector<std::vector<node *>> link;

    fp_tree(data &d);
};