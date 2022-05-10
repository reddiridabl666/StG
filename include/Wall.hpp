#pragma once

#include "DynamicObject.hpp"

class Wall : public DynamicObject {
  private:
    static sf::RenderTexture Rect;
    static const sf::Texture& get_rect(sf::Vector2f size, sf::Color color);

    float diag_y(float x, float tan) {
        return getPosition().y - (x - getPosition().x) * tan;
    };

    float diag_x(float y, float ctg) {
        return getPosition().x - (y - getPosition().y) * ctg;
    };

  public:
    explicit Wall(sf::Vector2f size = {0, 0}, sf::Vector2f pos = {0, 0}, 
         sf::Color color = sf::Color::Transparent)
        : DynamicObject(Wall::get_rect(size, color), pos, size, {0, 0}, 1) {
            setTag(Tag::Wall);
            hitbox_->setFillColor(sf::Color::Black);
    }

    bool is_in_right_sector(DynamicObject* obj) {
        auto tan = getSize().y / getSize().x;
        return obj->getPosition().y >= diag_y(obj->getPosition().x, tan) && 
               obj->getPosition().y <= diag_y(obj->getPosition().x, -tan);
    }

    bool is_in_left_sector(DynamicObject* obj) {
        auto tan = getSize().y / getSize().x;
        return obj->getPosition().y <= diag_y(obj->getPosition().x, tan) && 
               obj->getPosition().y >= diag_y(obj->getPosition().x, -tan);
    }

    bool is_in_upper_sector(DynamicObject* obj) {
        auto ctg = getSize().x / getSize().y;
        return obj->getPosition().x <= diag_x(obj->getPosition().y, ctg) && 
               obj->getPosition().x >= diag_x(obj->getPosition().y, -ctg);
    }

    bool is_in_lower_sector(DynamicObject* obj) {
        auto ctg = getSize().x / getSize().y;
        return obj->getPosition().x >= diag_x(obj->getPosition().y, ctg) && 
               obj->getPosition().x <= diag_x(obj->getPosition().y, -ctg);
    }

    void on_collide(DynamicObject* obj) override {
	// DynamicObject::on_collide(obj);
        if (mass_ > 0 && obj->getMass() > 0 && obj->getTag() != Tag::Wall) {
            // TODO: Wall bounce seems to work, but it needs to be done cleaner

            // Edge bounce
            if (std::abs(obj->getPosition().x - getPosition().x) >= getHalfSize().x &&
                std::abs(obj->getPosition().y - getPosition().y) >= getHalfSize().y) {
                // obj->move(-obj->getVelocity() / 1000.f);
                obj->setVelocity(-obj->getVelocity());
            // Horizontal walls
            } else if (is_in_lower_sector(obj) || is_in_upper_sector(obj)) {
                // obj->move(0, -obj->getVelocity().y / 1000.f);
                obj->setVelocity(obj->getVelocity().x, -obj->getVelocity().y);
            // Vertical walls
            } else if (is_in_left_sector(obj) || is_in_right_sector(obj)) {
                // obj->move(-obj->getVelocity().x / 1000.f, 0);
                obj->setVelocity(-obj->getVelocity().x, obj->getVelocity().y);
            }

            float offset = 15;

            if (is_in_left_sector(obj)) {
                obj->setPosition(getPosition().x - getHalfSize().x - obj->getHalfSize().x - offset, obj->getPosition().y);
            } else if (is_in_right_sector(obj)) {
                obj->setPosition(getPosition().x + getHalfSize().x + obj->getHalfSize().x + offset, obj->getPosition().y);
            } else if (is_in_lower_sector(obj)) {
                obj->setPosition(obj->getPosition().x, getPosition().y + getHalfSize().y + obj->getHalfSize().y + offset);
            } else if (is_in_upper_sector(obj)) {
                obj->setPosition(obj->getPosition().x, getPosition().y - getHalfSize().y - obj->getHalfSize().y - offset);
            }

            // // Edge bounce
            // if (std::abs(obj->getPosition().x - getPosition().x) >= getHalfSize().x &&
            //     std::abs(obj->getPosition().y - getPosition().y) >= getHalfSize().y) {
            //     obj->move(-obj->getVelocity() / 1000.f);
            //     obj->setVelocity(-obj->getVelocity());
            // // Horizontal walls
            // } else if (std::abs(obj->getPosition().x - getPosition().x) < getHalfSize().x &&
            //         std::abs(obj->getPosition().y - getPosition().y) > getHalfSize().y) {
            //     obj->move(0, -obj->getVelocity().y / 1000.f);
            //     obj->setVelocity(obj->getVelocity().x, -obj->getVelocity().y);
            // // Vertical walls
            // } else if (std::abs(obj->getPosition().x - getPosition().x) > getHalfSize().x &&
            //         std::abs(obj->getPosition().y - getPosition().y) < getHalfSize().y) {
            //     obj->move(-obj->getVelocity().x / 1000.f, 0);
            //     obj->setVelocity(-obj->getVelocity().x, obj->getVelocity().y);
            // }
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
