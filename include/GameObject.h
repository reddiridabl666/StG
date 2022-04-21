#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include <SFML/Graphics.hpp>

class GameObject {
  protected:
    sf::Texture *texture_;
    sf::Sprite sprite_;
    sf::Vector2f size_;
    bool is_active_ = true;

  public:
    sf::Uint8 layer_ = 0;
    using objects = std::unordered_set<GameObject*>;

    static objects all_objects;
    static std::vector<objects> objects_by_layer;

    static void draw_all(sf::RenderWindow& window);
    static void scale_all(float factor);

    GameObject();
    explicit GameObject(sf::Texture* texture, sf::Uint8 layer = 0);
    // explicit GameObject(const std::string& image_path, uint32_t layer = 0);

    // void loadFromFile(const std::string& image_path);
    const sf::Sprite& getSprite() const;
    sf::Vector2f getSize() const;
    void setTexture(sf::Texture *texture);

    void setPosition(float x, float y);
    void setPosition(sf::Vector2f offset);
    void move(float x, float y);
    void move(sf::Vector2f offset);
    void scale(float factor);

    void activate();
    void deactivate();
    bool is_active() const;

    void change_layer(sf::Uint8 layer);

    ~GameObject();
};
