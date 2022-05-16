#pragma once

#include "DynamicObject.hpp"
#include <memory>

using TexturePtr = std::unique_ptr<sf::RenderTexture>;

// using Frame = std::array<Wall, 4>;

struct Frame;

class Wall : public DynamicObject {
private:
    static std::list<TexturePtr> wall_textures;
    static const TexturePtr* get_rect(sf::Vector2f size, sf::Color color = sf::Color::Transparent);

public:
    static Frame get_frame(const sf::RenderWindow& window, float bound_width = 300);

    explicit Wall(sf::Vector2f size, sf::Vector2f pos = {0, 0}, 
         sf::Color color = sf::Color::Transparent);

    explicit Wall(const sf::Texture& texture, sf::Vector2f pos = {0, 0});

    Wall(const Wall& other) : DynamicObject(other) {}
    
    Wall& operator=(const Wall& other) {
        DynamicObject::operator=(other);
        return *this;    
    }

    bool is_in_right_sector(DynamicObject* obj);
    bool is_in_left_sector(DynamicObject* obj);
    bool is_in_upper_sector(DynamicObject* obj);
    bool is_in_lower_sector(DynamicObject* obj);

    void on_collide(DynamicObject* obj) override;

    // TODO: Wall textures don't work really well
    // void on_collide_stop() override {
    //     hitbox_->setFillColor(sf::Color::Black);
    // }
};

struct Frame {
	Wall left;
	Wall right;
	Wall up;
	Wall low;
};
