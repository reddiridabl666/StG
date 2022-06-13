#pragma once

#include <unordered_set>

#include "DrawableObject.hpp"

class GameObject : public DrawableObject {
protected:
    sf::Vector2f size_;
    bool is_active_ = true;

public:
    explicit GameObject(Layer layer = Layer::Bg, const sf::Vector2f& size = {0, 0});
    GameObject(const GameObject& other);

    static std::unordered_set<GameObject*> all;

    static void update_all(float deltaTime);

    void activate();
    void deactivate();
    bool is_active() const;

    virtual sf::Vector2f getHalfSize() const;
    virtual sf::Vector2f getSize() const;

    virtual void update(float);

    virtual ~GameObject();
};
