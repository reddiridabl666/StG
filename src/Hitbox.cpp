#include "Hitbox.h"
#include <set>

static const sf::Color green = {5, 240, 75};
static const sf::Color transp_green = {5, 240, 75, 200};
static const sf::Color red = {191, 34, 51};
static const sf::Color transp_red = {191, 34, 51, 200};


std::set<Hitbox*> Hitbox::all;

Hitbox::Hitbox(Layer layer, float mass) : GameObjectBase(layer), mass_(mass) {
    all.insert(this);
}

Hitbox::Hitbox(const Hitbox& hitbox) : GameObjectBase(hitbox.layer_) {
    mass_ = hitbox.mass_;
    all.insert(this);
}

bool Hitbox::collides_with(const Hitbox* other) {
    if (auto target = dynamic_cast<const RectHitbox*>(other)) {return collides_with_rect(target);}
    if (auto target = dynamic_cast<const CircleHitbox*>(other)) {return collides_with_circle(target);}
    return false;
}

void Hitbox::on_collide() {
    collision_num_++;
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
    
    for (auto it = all.begin(); it != all.end(); ++it) {
        // std::cout << "Checking collisions of hitbox " << *it << std::endl;
        for (auto jt = std::next(it); jt != all.end(); ++jt) {
            // std::cout << "With hitbox " << *jt << std::endl;
            if ((*it)->collides_with(*jt)) {
                (*it)->on_collide();
                (*jt)->on_collide();
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
  : Hitbox(layer), sf::RectangleShape(size) {
    setOrigin(size.x / 2.f, size.y / 2.f);
    setPosition(center);
    setOutlineThickness(-4);
    setOutlineColor(green);
    setFillColor(transp_green);
}

bool RectHitbox::contains_point(const sf::Vector2f& point) const {
    // return (point.y <= top_ + height_) && (point.y >= top_) && 
    //   (point.x <= left_ + width_) && (point.x >= left_);
    return getGlobalBounds().contains(point);
}

bool RectHitbox::collides_with_rect(const RectHitbox* other) const {
    // if ((top_ >= other->top_ && top_ <= other->top_ + other->height_) ||
    //     (other->top_ >= top_ && other->top_ <= top_ + height_)) {
    //     if ((left_ >= other->left_ && left_ <= other->left_ + other->width_) ||
    //         (other->left_ >= left_ && other->left_ <= left_ + width_)) {
    //             return true;
    //     }
    // }
    // return false;
    return getGlobalBounds().intersects(other->getGlobalBounds());
}

bool RectHitbox::collides_with_circle(const CircleHitbox* other) const {
    return other->collides_with_rect(this);
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

    auto rect = other->getGlobalBounds();

    if (contains_point({rect.left, rect.top}) || 
        contains_point({rect.left, rect.top + rect.height}) ||
        contains_point({rect.left + rect.width, rect.top}) || 
        contains_point({rect.left + rect.width, rect.top + rect.height})) {
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

sf::Drawable* CircleHitbox::get_drawable() {
    return this;
}
