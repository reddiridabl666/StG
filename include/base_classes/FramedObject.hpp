#pragma once

#include "DynamicObject.hpp"

class FramedObject : public DynamicObject {
protected:
    HitboxPtr frame_hitbox_;
public:
    static std::unordered_set<FramedObject*> all;

    FramedObject(Layer layer = Layer::Character) : DynamicObject(layer) {
        all.insert(this);
    }

    FramedObject(const FramedObject& other);

    FramedObject& operator=(const FramedObject& other);

    explicit FramedObject(const sf::Texture& texture, sf::Vector2f pos = {0, 0},
                    sf::Vector2f velocity = {0, 0}, float mass = 0,
                    Layer layer = Layer::Character);
    
    FramedObject(const sf::Texture& texture, sf::Vector2f pos, const HitboxInfo& hitbox_size,
                    sf::Vector2f velocity = {0, 0}, float mass = 0,
                    Layer layer = Layer::Character);

    explicit FramedObject(const sf::Texture& texture, sf::Vector2f pos,
                          const HitboxInfo& hitbox_size = 0, sf::Vector2f velocity = {0, 0}, 
                  float mass = 0, const HitboxInfo& phys_size = 0, Layer layer = Layer::Character);

    using DynamicObject::setPosition;

    void setPosition(const sf::Vector2f& pos) override;

    Hitbox* getFrame() {
        return frame_hitbox_.get();
    }

    void setRotation(float angle) override {
        DynamicObject::setRotation(angle);
        if (frame_hitbox_) {
            frame_hitbox_->getTransformable()->setRotation(angle);
        }
    }

    void on_collide(DynamicObject* obj) override;

    void show() override;

    void hide() override;

    void scale(float a, float b) override;


    ~FramedObject() override {
        all.erase(this);
    }
};
