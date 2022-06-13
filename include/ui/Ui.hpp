#pragma once

#include <unordered_set>
#include "DrawableObject.hpp"

class Ui : public DrawableObject {
private:
    static inline std::unordered_set<Ui*> all;
public:
    Ui() : DrawableObject(Layer::Ui) {
        all.insert(this);
    }

    virtual void update() {}

    static void update_all() {
        for (auto it : all) {
            if (!it->is_hidden())
                it->update();
        }
    }

    ~Ui() {
        all.erase(this);
    }
};
