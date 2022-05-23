#pragma once

#include "GameObject.h"

class SpriteObject : public GameObject, public sf::Sprite {
public:
    explicit SpriteObject(Layer layer = Layer::Bg);
    SpriteObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, Layer layer = Layer::Bg);
    SpriteObject(const SpriteObject& other);

    void setTexture(const sf::Texture& texture);

    sf::Vector2f getSize() const override;
    sf::Vector2f getHalfSize() const override;
    
    virtual void scale(float a, float b);
    void scale(float a);

    sf::Drawable* getDrawable() override;
    sf::Transformable* getTransformable() override;
};
