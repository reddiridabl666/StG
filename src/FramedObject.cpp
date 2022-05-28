#include "FramedObject.hpp"

std::unordered_set<FramedObject*> FramedObject::all;

FramedObject::FramedObject(const FramedObject& other) : DynamicObject(other) {
    if (other.frame_hitbox_) {
        frame_hitbox_ = Hitbox::getFrameHitbox(other.frame_hitbox_->getInfo(), 
                                                other.getPosition());
    }
    all.insert(this);
}

FramedObject& FramedObject::operator=(const FramedObject& other) {
    DynamicObject::operator=(other);
    if (other.frame_hitbox_) {
        frame_hitbox_ = Hitbox::getFrameHitbox(other.frame_hitbox_->getInfo(),
                                                other.getPosition());
    }
    setPosition(other.getPosition());
    return *this;
}

FramedObject::FramedObject(const sf::Texture& texture, sf::Vector2f pos,
                sf::Vector2f velocity, float mass, Layer layer) : 
    DynamicObject(texture, pos, velocity, mass, layer) {
    all.insert(this);
}

FramedObject::FramedObject(const sf::Texture& texture, sf::Vector2f pos, const HitboxInfo& hitbox_size,
                           sf::Vector2f velocity, float mass, Layer layer) : 
    FramedObject(texture, pos, hitbox_size, velocity, mass, static_cast<sf::Vector2f>(texture.getSize()),  layer) {}

FramedObject::FramedObject(const sf::Texture& texture, sf::Vector2f pos,
                           const HitboxInfo& hitbox_size, sf::Vector2f velocity, 
                           float mass, const HitboxInfo& phys_size, Layer layer) : 
    DynamicObject(texture, pos, hitbox_size, velocity, mass, layer), 
    frame_hitbox_(Hitbox::getFrameHitbox(phys_size, pos)) {
    all.insert(this);
}

void FramedObject::setPosition(const sf::Vector2f& pos) {
    DynamicObject::setPosition(pos);
    if (frame_hitbox_) {
        frame_hitbox_->getTransformable()->setPosition(pos);
    }
}

void FramedObject::on_collide(DynamicObject* obj) {
    if (!obj)
        return;
    // if (obj && frame_hitbox_) {
    //     frame_hitbox_->on_collide();
    // }
}

void FramedObject::show() {
    DynamicObject::show();
#ifdef DEBUG
    if (frame_hitbox_) {
        frame_hitbox_->show();
    }
#endif
}

void FramedObject::hide() {
    DynamicObject::hide();
    if (frame_hitbox_) {
        frame_hitbox_->hide();
    }
}

void FramedObject::scale(float a, float b) {
    DynamicObject::scale(a, b);
    if (frame_hitbox_) {
        frame_hitbox_->scale(a, b);
    }
}

bool FramedObject::collides_with_phys(DynamicObject* obj) {
    if (frame_hitbox_ && obj)
        return frame_hitbox_->collides_with(obj->getHitbox());
    return false;
}

void FramedObject::check_phys_collisions_with(DynamicObject& other) {
    for (auto it : all) {
        if (!it) continue;

        if (it != &other) {
            if (it->collides_with_phys(&other)) {
                it->on_collide(&other);
                other.on_collide(it);
            }
        }
        
        if (it->frame_hitbox_ && it->getFrame()->getCollisionNum() == 0) {
            it->on_collide_stop();
        }
    }
    if (other.getHitbox() && other.getHitbox()->getCollisionNum() == 0) {
        other.on_collide_stop();
    }
}
