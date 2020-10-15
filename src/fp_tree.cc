#include "fp_tree.h"

fp_tree::node *fp_tree::node::find(data::item item) const {
    for (auto i : child)
        if (i->item == item)
            return i;
    return nullptr;
}

fp_tree::node *fp_tree::node::add_child(data::item item) {
    auto n = new node(item, this);
    child.push_back(n);
    return n;
}

fp_tree::fp_tree(data &d) : link(d.num_items()) {
    for (const auto t : d.transactions()) {
        node *current = &root;
        for (const auto &i : t) {
            node *child = current->find(static_cast<int>(i));
            if (child == nullptr) {
                child = current->add_child(i);
                link[static_cast<int>(i)].push_back(child);
            }
            ++child->freq;
            current = child;
        }
    }
}