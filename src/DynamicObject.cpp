#include "DynamicObject.hpp"

std::set<DynamicObject*> DynamicObject::all;

DynamicObject::DynamicObject(const DynamicObject& rhs) : DynamicObject(*rhs.getTexture(), rhs.getPosition(), 
                                                         rhs.velocity_, rhs.mass_, rhs.layer_) {
    hitbox_ = Hitbox::getHitbox(rhs.getHitbox()->getInfo());
    setTag(rhs.getTag());
}

DynamicObject& DynamicObject::operator=(const DynamicObject& rhs) {
    DynamicObject temp(rhs);
    swap(temp);
    // if (rhs.getTexture())
    //     setTexture(*rhs.getTexture());
    // if (hitbox_) {
    //     delete hitbox_;
    // }
    // setVelocity(rhs.getVelocity());
    // setMass(rhs.getMass());
    // setTag(rhs.getTag());
    // change_layer(rhs.layer_);
    // hitbox_ = Hitbox::getHitbox(rhs.getHitbox()->getInfo());
    // setPosition(rhs.getPosition());
    return *this;
}

void DynamicObject::swap(DynamicObject& other) {
        std::swap(hitbox_, other.hitbox_);
        std::swap(velocity_, other.velocity_);
        std::swap(mass_, other.mass_);
        std::swap(tag_, other.tag_);
        std::swap(size_, other.size_);

        auto layer = layer_;
        change_layer(other.layer_);
        other.change_layer(layer);

        auto pos = getPosition();
        setPosition(other.getPosition());
        other.setPosition(pos);

        const_cast<sf::Texture*>(getTexture())->swap(*const_cast<sf::Texture*>(other.getTexture()));
}
// DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
//                  const sf::Vector2f& hitbox_size, sf::Vector2f velocity, 
//                  float mass, Layer layer) 
//     : DynamicObject(texture, pos, velocity, mass, layer) {
//     hitbox_ = new RectHitbox(hitbox_size, pos);
// }

DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                 const HitboxInfo& hitbox_size, sf::Vector2f velocity, 
                 float mass, Layer layer) 
    : DynamicObject(texture, pos, velocity, mass, layer) {
    hitbox_ = Hitbox::getHitbox(hitbox_size, pos);
}

// DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
//                     float hitbox_radius, sf::Vector2f velocity, 
//                     float mass, Layer layer) 
//     : DynamicObject(texture, pos, velocity, mass, layer) {    
//     hitbox_ = new CircleHitbox(hitbox_radius, pos);
// }

void DynamicObject::move_all(float deltaTime) {
    // for (auto it : all) {
    //     it->move(it->velocity_ * deltaTime);
    // }
    for_each([deltaTime] (DynamicObject* obj) {
        obj->move(obj->velocity_ * deltaTime);
    });
}

inline void DynamicObject::for_each(std::function<void(DynamicObject*)> action) {
    for (auto it : all) {
        if (!it) continue;
        action(it);
    }
}

void DynamicObject::on_collide(DynamicObject* obj) {
    if (obj && hitbox_)
        hitbox_->on_collide();
}

void DynamicObject::check_collisions_with(DynamicObject& other) {
    DynamicObject::refresh_collision_num();

    for (auto it : all) {
        if (!it) continue;

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
            if (it && it->hitbox_)
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
        if (*it && (*it)->hitbox_ && (*it)->hitbox_->collision_num_ == 0) {
            (*it)->on_collide_stop();
        }
    }
}
