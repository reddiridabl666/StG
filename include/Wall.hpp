#pragma once

#include "DynamicObject.hpp"

struct Frame;

class Wall : public DynamicObject {
  private:
    static sf::RenderTexture Rect;
    static const sf::Texture& get_rect(sf::Vector2f size, sf::Color color);
  public:
    static Frame Bounds; 
    
    explicit Wall(sf::Vector2f size = {0, 0}, sf::Vector2f pos = {0, 0}, 
         sf::Color color = sf::Color::Transparent)
        : DynamicObject(Wall::get_rect(size, color), pos, size, {0, 0}, 1) {
            setTag(Tag::Wall);
    }

  void on_collide(DynamicObject* obj) override {
	DynamicObject::on_collide(obj);
	
    if (mass_ > 0 && obj->getMass() > 0) {
      if (std::abs(obj->getPosition().x - getPosition().x) >= getSize().x / 2 &&
          std::abs(obj->getPosition().y - getPosition().y) <= getSize().y / 2) {
        	obj->setVelocity(-obj->getVelocity().x, obj->getVelocity().y);
      } else {
            obj->setVelocity(obj->getVelocity().x, -obj->getVelocity().y);
      }
    }
  }

};

struct Frame {
    Wall left;
    Wall right;
    Wall up;
    Wall low;
};

inline sf::RenderTexture Wall::Rect;

inline const sf::Texture& Wall::get_rect(sf::Vector2f size, sf::Color color) {
    Rect.create(size.x, size.y);
    sf::RectangleShape rect(size);
    rect.setFillColor(color);
    Rect.draw(rect);
    return Rect.getTexture();
}

