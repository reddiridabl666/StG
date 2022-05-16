#include "Wall.hpp"

std::list<TexturePtr> Wall::wall_textures;

static inline float diag_y(const sf::Transformable* obj, float x, float tan) {
        return obj->getPosition().y - (x - obj->getPosition().x) * tan;
}

static inline float diag_x(const sf::Transformable* obj, float y, float ctg) {
    return obj->getPosition().x - (y - obj->getPosition().y) * ctg;
}

Wall::Wall(sf::Vector2f size, sf::Vector2f pos, sf::Color color) : 
        DynamicObject((*get_rect(size, color))->getTexture(), pos, size, {0, 0}, 1) {
        setTag(Tag::Wall);
}

Wall::Wall(const sf::Texture& texture, sf::Vector2f pos) : 
    DynamicObject(texture, pos, static_cast<sf::Vector2f>(texture.getSize()), {0, 0}, 1) {
    setTag(Tag::Wall);
}

bool Wall::is_in_right_sector(DynamicObject* obj) {
    auto tan = getSize().y / getSize().x;
    return obj->getPosition().y >= diag_y(this, obj->getPosition().x, tan) && 
           obj->getPosition().y <= diag_y(this, obj->getPosition().x, -tan);
}

bool Wall::is_in_left_sector(DynamicObject* obj) {
    auto tan = getSize().y / getSize().x;
    return obj->getPosition().y <= diag_y(this, obj->getPosition().x, tan) && 
           obj->getPosition().y >= diag_y(this, obj->getPosition().x, -tan);
}

bool Wall::is_in_upper_sector(DynamicObject* obj) {
    auto ctg = getSize().x / getSize().y;
    return obj->getPosition().x <= diag_x(this, obj->getPosition().y, ctg) && 
           obj->getPosition().x >= diag_x(this, obj->getPosition().y, -ctg);
}

bool Wall::is_in_lower_sector(DynamicObject* obj) {
    auto ctg = getSize().x / getSize().y;
    return obj->getPosition().x >= diag_x(this, obj->getPosition().y, ctg) && 
           obj->getPosition().x <= diag_x(this, obj->getPosition().y, -ctg);
}

void Wall::on_collide(DynamicObject* obj) {
// DynamicObject::on_collide(obj);
    if (mass_ > 0 && obj->getMass() > 0 && obj->getTag() != Tag::Wall) {
        // Edge bounce
        if (std::abs(obj->getPosition().x - getPosition().x) >= getHalfSize().x &&
            std::abs(obj->getPosition().y - getPosition().y) >= getHalfSize().y) {
            obj->setVelocity(-obj->getVelocity());
        // Horizontal walls
        } else if (is_in_lower_sector(obj) || is_in_upper_sector(obj)) {
            obj->setVelocity(obj->getVelocity().x, -obj->getVelocity().y);
        // Vertical walls
        } else if (is_in_left_sector(obj) || is_in_right_sector(obj)) {
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
    }
}

const TexturePtr* Wall::get_rect(sf::Vector2f size, sf::Color color) {
    wall_textures.emplace_back(std::make_unique<sf::RenderTexture>());
    wall_textures.back()->create(size.x, size.y);
    wall_textures.back()->clear(color);
    wall_textures.back()->display();
    return &wall_textures.back();
}

Frame Wall::get_frame(const sf::RenderWindow& window, float bound_width) {
    auto win_size = window.getView().getSize();

    auto vertical = Wall::get_rect({bound_width, win_size.y});
    auto horizontal = Wall::get_rect({win_size.x, bound_width});

    Wall left_bound(vertical->get()->getTexture(), {-bound_width / 2, win_size.y / 2});
    Wall right_bound(vertical->get()->getTexture(), {win_size.x + bound_width / 2, win_size.y / 2});
    Wall up_bound(horizontal->get()->getTexture(), {win_size.x / 2, -bound_width / 2});
    Wall low_bound(horizontal->get()->getTexture(), {win_size.x / 2, win_size.y + bound_width / 2});
    return {left_bound, right_bound, up_bound, low_bound};
}
