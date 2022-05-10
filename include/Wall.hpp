#pragma once

#include "DynamicObject.hpp"

class Wall : public DynamicObject {
  private:
    static sf::RenderTexture Rect;
    static const sf::Texture& get_rect(sf::Vector2f size, sf::Color color);
  public:
    explicit Wall(sf::Vector2f size = {0, 0}, sf::Vector2f pos = {0, 0}, 
         sf::Color color = sf::Color::Transparent)
        : DynamicObject(Wall::get_rect(size, color), pos, size, {0, 0}, 1) {
            setTag(Tag::Wall);
            hitbox_->setFillColor(sf::Color::Black);
    }

  void on_collide(DynamicObject* obj) override {
	// DynamicObject::on_collide(obj);
	
    if (mass_ > 0 && obj->getMass() > 0 && obj->getTag() != Tag::Wall) {
		// TODO: Wall bounce seems to work, but it needs to be done cleaner

		// Edge bounce
		if (std::abs(obj->getPosition().x - getPosition().x) >= getSize().x / 2 &&
			std::abs(obj->getPosition().y - getPosition().y) >= getSize().y / 2) {
				obj->move(-obj->getVelocity() / 1000.f);
				obj->setVelocity(-obj->getVelocity());
		// Horizontal walls
		} else if (std::abs(obj->getPosition().x - getPosition().x) < getSize().x / 2 &&
					std::abs(obj->getPosition().y - getPosition().y) > getSize().y / 2) {
				obj->move(0, -obj->getVelocity().y / 1000.f);
				obj->setVelocity(obj->getVelocity().x, -obj->getVelocity().y);
		// Vertical walls
		} else if (std::abs(obj->getPosition().x - getPosition().x) > getSize().x / 2 &&
					std::abs(obj->getPosition().y - getPosition().y) < getSize().y / 2) {
				obj->move(-obj->getVelocity().x / 1000.f, 0);
				obj->setVelocity(-obj->getVelocity().x, obj->getVelocity().y);
		}
    }
  }

  // Doesn't work for some reason
  void on_collide_stop() override {
    hitbox_->setFillColor(sf::Color::Black);
  }
};

inline sf::RenderTexture Wall::Rect;

inline const sf::Texture& Wall::get_rect(sf::Vector2f size, sf::Color color) {
    Rect.create(size.x, size.y);
    sf::RectangleShape rect(size);
    rect.setFillColor(color);
    Rect.draw(rect);
    return Rect.getTexture();
}

