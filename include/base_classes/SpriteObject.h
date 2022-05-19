#pragma once

#include "GameObject.h"

class SpriteObject : public GameObject, public sf::Sprite {
  protected:
    sf::Vector2f size_;

  public:
    explicit SpriteObject(Layer layer = Layer::Bg);
    SpriteObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0}, Layer layer = Layer::Bg);
    SpriteObject(const SpriteObject& other);

    sf::Vector2f getSize() const;
    sf::Vector2f getHalfSize() const override;
    void setTexture(const sf::Texture& texture);

    sf::Drawable* getDrawable() override;
    sf::Transformable* getTransformable() override;
};
