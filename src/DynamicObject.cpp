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

DynamicObject::DynamicObject(const sf::Texture& texture, sf::Vector2f pos, 
                 const HitboxInfo& hitbox_size, sf::Vector2f velocity, 
                 float mass, Layer layer) 
    : DynamicObject(texture, pos, velocity, mass, layer) {
    hitbox_ = Hitbox::getHitbox(hitbox_size, pos);
}

void DynamicObject::on_collide(DynamicObject* obj) {
    if (obj && hitbox_)
        hitbox_->on_collide();
}

void DynamicObject::refresh_collision_num() {
    for (auto it : DynamicObject::all) {
            if (it && it->hitbox_)
                it->hitbox_->collision_num_ = 0;
    }
}
