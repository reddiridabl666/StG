#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "Utilities.hpp"

class GameObjectBase {
  protected:
    Layer layer_;
    bool is_active_ = true;

  public:
    explicit GameObjectBase(Layer layer = Layer::bg);
    using objects = std::unordered_set<GameObjectBase*>;

    static objects all_objects;
    static std::unordered_map<Layer, objects> objects_by_layer;

    static void draw_all(sf::RenderWindow& window);
    static void scale_all(float factor);

    virtual void scale(float factor) = 0;

    void activate();
    void deactivate();
    bool is_active() const;

    virtual sf::Drawable* get_drawable() = 0;
    void change_layer(Layer layer);

    virtual ~GameObjectBase();
};

class GameObject : public GameObjectBase, public sf::Sprite {
  protected:
    sf::Vector2f size_;

  public:
    explicit GameObject(Layer layer = Layer::bg);
    GameObject(const sf::Texture& texture, Layer layer = Layer::bg);

    sf::Vector2f getSize() const;
    void setTexture(const sf::Texture& texture);
    void scale(float factor) override;

    sf::Drawable* get_drawable() override;

    ~GameObject() override;
};
