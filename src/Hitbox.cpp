#include "Hitbox.h"
#include <set>

static const sf::Color green = {5, 240, 75};
static const sf::Color transp_green = {5, 240, 75, 200};
static const sf::Color red = {191, 34, 51};
static const sf::Color transp_red = {191, 34, 51, 200};


std::set<Hitbox*> Hitbox::all;

Hitbox::Hitbox(Layer layer) : GameObjectBase(layer) {
    all.insert(this);
}

bool Hitbox::collides_with(const Hitbox* other) {
    if (auto target = dynamic_cast<const RectHitbox*>(other)) {return collides_with_rect(target);}
    if (auto target = dynamic_cast<const CircleHitbox*>(other)) {return collides_with_circle(target);}
    return false;
}

void Hitbox::on_collide() {
    dynamic_cast<sf::Shape*>(get_drawable())->setOutlineColor(red);
    dynamic_cast<sf::Shape*>(get_drawable())->setFillColor(transp_red);
}

void Hitbox::on_collide_stop() {
    dynamic_cast<sf::Shape*>(get_drawable())->setOutlineColor(green);
    dynamic_cast<sf::Shape*>(get_drawable())->setFillColor(transp_green);
}

void Hitbox::refresh_collision_num() {
    for (auto it : Hitbox::all) {
        it->collision_num_ = 0;
    }
}

void Hitbox::check_collisions() {
    refresh_collision_num();
    for (auto it = all.begin(); it != std::prev(all.end()); ++it) {
        // std::cout << "Checking collisions of hitbox " << *it << std::endl;
        for (auto jt = std::next(it); jt != all.end(); ++jt) {
            // std::cout << "With hitbox " << *jt << std::endl;
            if ((*it)->collides_with(*jt)) {
                (*it)->on_collide();
                (*jt)->on_collide();

                (*it)->collision_num_++;
                (*jt)->collision_num_++;
            }

            if ((*jt)->collision_num_ == 0) {
                (*jt)->on_collide_stop();
            }
        }

        if ((*it)->collision_num_ == 0) {
            (*it)->on_collide_stop();
        }
    }
} 

Hitbox::~Hitbox() {
    all.erase(this);
}

RectHitbox::RectHitbox(const sf::Vector2f &size, const sf::Vector2f &center, Layer layer) 
  : Hitbox(layer), sf::RectangleShape(size), height_(size.y), width_(size.x) {
    setOrigin(size.x / 2.f, size.y / 2.f);
    setPosition(center);
    setOutlineThickness(-4);
    setOutlineColor(green);
    setFillColor(transp_green);
}

bool RectHitbox::contains_point(const sf::Vector2f& point) const {
    return (point.y <= top_ + height_) && (point.y >= top_) && 
      (point.x <= left_ + width_) && (point.x >= left_);
}

bool RectHitbox::collides_with_rect(const RectHitbox* other) const {
    if ((top_ >= other->top_ && top_ <= other->top_ + other->height_) ||
        (other->top_ >= top_ && other->top_ <= top_ + height_)) {
        if ((left_ >= other->left_ && left_ <= other->left_ + other->width_) ||
            (other->left_ >= left_ && other->left_ <= left_ + width_)) {
                return true;
        }
    }
    return false;
}

bool RectHitbox::collides_with_circle(const CircleHitbox* other) const {
    return other->collides_with_rect(this);
}

void RectHitbox::setPosition(const sf::Vector2f& center) {
    sf::RectangleShape::setPosition(center);
    top_ = center.y - height_ / 2;
    left_ = center.x - width_ / 2;
}

void RectHitbox::setPosition(float x, float y) {
    setPosition({x, y});
}

void RectHitbox::move(const sf::Vector2f& offset) {
    setPosition(getPosition() + offset);
}

void RectHitbox::move(float x, float y) {
    move({x, y});
}

// void RectHitbox::on_collide() {
//     setOutlineColor(red);
//     setFillColor(transp_red);
// }

// void RectHitbox::on_collide_stop() {
//     setOutlineColor(green);
//     setFillColor(transp_green);
// }

void RectHitbox::scale(float factor) {
    sf::RectangleShape::scale(factor, factor);
}

sf::Drawable* RectHitbox::get_drawable() {
    return this;
}

CircleHitbox::CircleHitbox(float radius, const sf::Vector2f &center, Layer layer) 
  : Hitbox(layer), sf::CircleShape(radius) {
    setOrigin(radius, radius);
    setPosition(center);
    setOutlineThickness(-4);
    setOutlineColor(green);
    setFillColor(transp_green);
}
      
bool CircleHitbox::contains_point(const sf::Vector2f& point) const {
    return distance(getPosition(), point) <= getRadius();
}

bool CircleHitbox::collides_with_rect(const RectHitbox* other) const {
    // TODO: Сделать через проекции, пока криво

    if (contains_point({other->left_, other->top_}) || 
        contains_point({other->left_, other->top_ + other->height_}) ||
        contains_point({other->left_ + other->width_, other->top_}) || 
        contains_point({other->left_ + other->width_, other->top_ + other->height_})) {
            return true;
    }

    if (other->contains_point(getPosition() + sf::Vector2f{0, -getRadius()}) || 
        other->contains_point(getPosition() + sf::Vector2f{getRadius(), 0}) ||
        other->contains_point(getPosition() + sf::Vector2f{0, getRadius()}) || 
        other->contains_point(getPosition() + sf::Vector2f{-getRadius(), 0})) {
            return true;
    }

    return false;
}

bool CircleHitbox::collides_with_circle(const CircleHitbox* other) const {
    return distance(getPosition(), other->getPosition()) <= getRadius() + other->getRadius();
}

// void on_collide() override;
// virtual void on_collide_stop() override;

void CircleHitbox::scale(float factor) {
    sf::CircleShape::scale(factor, factor);
}

sf::Drawable* CircleHitbox::get_drawable() {
    return this;
}
