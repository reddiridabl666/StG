#include "DynamicObject.hpp"
#include "Wall.hpp"

std::set<DynamicObject*> DynamicObject::all;

DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                 const sf::Vector2f& hitbox_size, sf::Vector2f velocity, 
                 float mass, Layer layer) 
            : DynamicObject(texture, pos, velocity, mass, layer) {
            hitbox_ = new RectHitbox(hitbox_size, pos);
    }

DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                    float hitbox_radius, sf::Vector2f velocity, 
                    float mass, Layer layer) 
    : DynamicObject(texture, pos, velocity, mass, layer) {    
    hitbox_ = new CircleHitbox(hitbox_radius, pos);
}
void DynamicObject::move_all(float deltaTime) {
    // for (auto it : all) {
    //     it->move(it->velocity_ * deltaTime);
    // }
    for_each([deltaTime] (DynamicObject* obj) {
        obj->move(obj->velocity_ * deltaTime);
    });
}

void DynamicObject::for_each(std::function<void(DynamicObject*)> action) {
    for (auto it : all) {
        action(it);
    }
}

void DynamicObject::on_collide(const DynamicObject* obj) {
    if (obj->mass_ > 0 && mass_ > 0) {
        // if (obj->tag_ == Tag::Wall) {
        //     setVelocity(2.f * obj->velocity_ - velocity_);
        // }
    }
    if (obj->getTag() == Tag::Wall) {
        if (obj == &Wall::Bounds[0]) {
        setPosition(Wall::Bounds[0].getPosition().x + 
                    Wall::Bounds[0].getSize().x / 2 + hitbox_->getSize().x / 2,
                    getPosition().y);
        }
        if (obj == &Wall::Bounds[1]) {
            setPosition(Wall::Bounds[1].getPosition().x - 
                        Wall::Bounds[1].getSize().x / 2 - hitbox_->getSize().x / 2,
                        getPosition().y);
        }
        if (obj == &Wall::Bounds[2]) {
            setPosition(getPosition().x, Wall::Bounds[2].getPosition().y - 
                        Wall::Bounds[2].getSize().y / 2 + hitbox_->getSize().y / 2);
        }
        if (obj == &Wall::Bounds[3]) {
            setPosition(getPosition().x, Wall::Bounds[3].getPosition().y + 
                        Wall::Bounds[3].getSize().y / 2 - hitbox_->getSize().y / 2);
        }
    }
    if (hitbox_)
        hitbox_->on_collide();
}

void DynamicObject::check_collisions_with(DynamicObject& other) {
    DynamicObject::refresh_collision_num();

    for (auto it : all) {
        if (it != &other) {
            if (it->collides_with(other)) {
                it->on_collide(&other);
                other.on_collide(it);
            }
        }
        if (it->hitbox_ && it->hitbox_->collision_num_ == 0) {
            it->on_collide_stop();
        }
    }
    if (other.hitbox_ && other.hitbox_->collision_num_ == 0) {
        other.on_collide_stop();
    }
}

void DynamicObject::refresh_collision_num() {
    for (auto it : DynamicObject::all) {
            if (it->hitbox_)
                it->hitbox_->collision_num_ = 0;
    }
}

void DynamicObject::check_collisions() {
    DynamicObject::refresh_collision_num();
    
    for (auto it = all.begin(); it != all.end(); ++it) {
        // std::cout << "Checking collisions of hitbox " << *it << std::endl;
        for (auto jt = std::next(it); jt != all.end(); ++jt) {
            // std::cout << "With hitbox " << *jt << std::endl;
            if ((*it)->collides_with(*jt)) {
                (*it)->on_collide(*jt);
                (*jt)->on_collide(*it);
            }
        }
        if ( (*it)->hitbox_ && (*it)->hitbox_->collision_num_ == 0) {
            (*it)->hitbox_->on_collide_stop();
        }
    }
}
