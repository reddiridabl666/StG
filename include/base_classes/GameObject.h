#pragma once

#include <iostream>
#include <string>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "Utilities.hpp"

class GameObject {
  protected:
    Layer layer_;
    bool is_active_ = true;

  public:
    explicit GameObject(Layer layer = Layer::Bg);
    using objects = std::unordered_set<GameObject*>;

    static std::unordered_map<Layer, objects> objects_by_layer;
    static void draw_all(sf::RenderWindow& window);

    void activate();
    void deactivate();
    bool is_active() const;

    virtual sf::Drawable* getDrawable() = 0;
    virtual sf::Transformable* getTransformable() = 0;
    virtual sf::Vector2f getHalfSize() const = 0;

    void change_layer(Layer layer);

    virtual ~GameObject();
};
