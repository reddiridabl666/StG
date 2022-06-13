#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <unordered_map>

enum class Layer {
    Bg,
    Character,
    Hitbox,
    Bullet,
    Ui
};

class DrawableObject {
protected:
    Layer layer_;
    bool is_hidden_ = false;

public:
    using objects = std::unordered_set<DrawableObject*>;
    static std::unordered_map<Layer, objects> objects_by_layer;

    DrawableObject(Layer layer = Layer::Bg) : layer_(layer) {
        objects_by_layer[layer].insert(this);
    }

    virtual void hide() {
        is_hidden_ = true;
    }

    virtual void show() {
        is_hidden_ = false;
    }

    bool is_hidden() const {
        return is_hidden_;
    }

    void switch_show_mode() {
        is_hidden_ = 1 - (is_hidden_ % 2);
    }
    
    void change_layer(Layer layer) {
        objects_by_layer[layer_].erase(this);
        layer_ = layer;
        objects_by_layer[layer_].insert(this);
    }

    static void draw_all(sf::RenderWindow& window);

    virtual sf::Drawable* getDrawable() = 0;
    virtual sf::Transformable* getTransformable() = 0;

    ~DrawableObject() {
        objects_by_layer[layer_].erase(this);
    }
};
