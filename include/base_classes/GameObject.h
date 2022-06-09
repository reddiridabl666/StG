#pragma once

#include <iostream>
#include <string>
#include <list>
#include <unordered_set>
#include <unordered_map>

#include <SFML/Graphics.hpp>

enum class Layer {
    Bg,
    Character,
    Hitbox,
    Bullet,
    Interface
};

class GameObject {
protected:
    Layer layer_;
    sf::Vector2f size_;
    bool is_active_ = true;
    bool is_hidden_ = false;

public:
    explicit GameObject(Layer layer = Layer::Bg, const sf::Vector2f& size = {0, 0});
    GameObject(const GameObject& other);
    using objects = std::unordered_set<GameObject*>;

    static std::unordered_map<Layer, objects> objects_by_layer;
    static void draw_all(sf::RenderWindow& window);

    static void update_all(float deltaTime);

    void activate();
    void deactivate();
    bool is_active() const;

    virtual void hide();
    virtual void show();
    bool is_hidden() const;
    void switch_show_mode();

    virtual sf::Drawable* getDrawable() = 0;
    virtual sf::Transformable* getTransformable() = 0;
    virtual sf::Vector2f getHalfSize() const;
    virtual sf::Vector2f getSize() const;

    virtual void update(float);

    void change_layer(Layer layer);

    virtual ~GameObject();
};
