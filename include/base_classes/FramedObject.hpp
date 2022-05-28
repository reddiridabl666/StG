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
        return frame_hitbox_;
    }

    void on_collide(DynamicObject* obj) override;

    void show() override;

    void hide() override;

    void scale(float a, float b) override;

    // virtual void on_collide_phys(FramedObject* obj) {}
    
    virtual bool collides_with_phys(DynamicObject* obj);

    static void check_phys_collisions_with(DynamicObject& other);

    ~FramedObject() override {
        if (frame_hitbox_) {
            delete frame_hitbox_;
        }
        all.erase(this);
    }
};
