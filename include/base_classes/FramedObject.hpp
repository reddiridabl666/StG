#pragma once

#include "DynamicObject.hpp"

class FramedObject : public DynamicObject {
protected:
    // float mass_;
    Hitbox* frame_hitbox_ = nullptr;
    static std::unordered_set<FramedObject*> all; // Куча копипасты
public:
    FramedObject(Layer layer = Layer::Character) : DynamicObject(layer) {
        all.insert(this);
    }

    FramedObject(const FramedObject& other) : DynamicObject(other) {
        if (other.frame_hitbox_) {
            frame_hitbox_ = Hitbox::getFrameHitbox(other.frame_hitbox_->getInfo(), 
                                                 other.getPosition());
        }
        all.insert(this);
    }

    FramedObject& operator=(const FramedObject& other) {
        DynamicObject::operator=(other);
        if (other.frame_hitbox_) {
            frame_hitbox_ = Hitbox::getFrameHitbox(other.frame_hitbox_->getInfo(),
                                                 other.getPosition());
        }
        setPosition(other.getPosition());
        return *this;
    }

    explicit FramedObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
                    sf::Vector2f velocity = {0, 0}, float mass = 0,
                    Layer layer = Layer::Character) : 
        DynamicObject(texture, pos, velocity, mass, layer) {
        all.insert(this);
    }
    
    FramedObject(const sf::Texture& texture, sf::Vector2f pos, const HitboxInfo& hitbox_size,
                    sf::Vector2f velocity = {0, 0}, float mass = 0,
                    Layer layer = Layer::Character) : 
        FramedObject(texture, pos, hitbox_size, velocity, mass, static_cast<sf::Vector2f>(texture.getSize()),  layer) {}

    explicit FramedObject(const sf::Texture& texture, sf::Vector2f pos,
                          const HitboxInfo& hitbox_size = 0, sf::Vector2f velocity = {0, 0}, 
                  float mass = 0, const HitboxInfo& phys_size = 0, Layer layer = Layer::Character) : 
        DynamicObject(texture, pos, hitbox_size, velocity, mass, layer), 
        frame_hitbox_(Hitbox::getFrameHitbox(phys_size, pos)) {
        all.insert(this);
    }

    using DynamicObject::setPosition;

    void setPosition(const sf::Vector2f& pos) override {
        DynamicObject::setPosition(pos);
        if (frame_hitbox_) {
            frame_hitbox_->getTransformable()->setPosition(pos);
        }
    }

    Hitbox* getFrame() {
        return frame_hitbox_;
    }

    void on_collide(DynamicObject* obj) override {
        if (!obj)
            return;
        // if (obj && frame_hitbox_) {
        //     frame_hitbox_->on_collide();
        // }
    }

    void show() override {
        DynamicObject::show();
#ifdef DEBUG
        if (frame_hitbox_) {
            frame_hitbox_->show();
        }
#endif
    }

    void hide() override {
        DynamicObject::hide();
        if (frame_hitbox_) {
            frame_hitbox_->hide();
        }
    }

    void scale(float a, float b) override {
        DynamicObject::scale(a, b);
        if (frame_hitbox_) {
            frame_hitbox_->scale(a, b);
            // dynamic_cast<sf::Shape*>(frame_hitbox_)->setOutlineThickness(-4);
        }
    }

    // virtual void on_collide_phys(FramedObject* obj) {}
    
    virtual bool collides_with_phys(DynamicObject* obj) {
        if (frame_hitbox_ && obj)
            return frame_hitbox_->collides_with(obj->getHitbox());
        return false;
    }

    static void check_phys_collisions_with(DynamicObject& other) {
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

    ~FramedObject() override {
        if (frame_hitbox_) {
            delete frame_hitbox_;
        }
        all.erase(this);
    }
};

inline std::unordered_set<FramedObject*> FramedObject::all;
