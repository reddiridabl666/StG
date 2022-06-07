#pragma once

#include "DynamicObject.hpp"
#include "FramedObject.hpp"
#include "Player.hpp"
#include <memory>

using TexturePtr = std::unique_ptr<sf::RenderTexture>;

// using Frame = std::array<Wall, 4>;

struct Frame;

class Wall : public RectHitbox {
private:
    // static std::list<TexturePtr> wall_textures;
    // static const TexturePtr* get_rect(sf::Vector2f size, sf::Color color = sf::Color::Transparent);

public:
    static Frame get_frame(const sf::RenderWindow& window, float bound_width = 300);

    explicit Wall(sf::Vector2f size, sf::Vector2f pos = {0, 0});
         
    Wall() : RectHitbox() {}

    // explicit Wall(const sf::Vector2f size = {0, 0}, sf::Vector2f pos = {0, 0});

    // Wall(const Wall& other) : RectHitbox(other) {
    //     size_ = other.getSize();
    // }
    
    // Wall& operator=(const Wall& other) {
    //     RectHitbox::operator=(other);
    //     return *this;    
    // }

    bool is_in_right_sector(const DynamicObject& obj) const;
    bool is_in_left_sector(const DynamicObject& obj) const;
    bool is_in_upper_sector(const DynamicObject& obj) const;
    bool is_in_lower_sector(const DynamicObject& obj) const;

    void on_collide(FramedObject& obj);

    void check_collisions_with(const std::unordered_set<FramedObject*>& objs);

    void player_collision(Player& player);
    // void on_player_collide(Player& player);
    void on_player_collide(Player& player);
};

// class Wall : public DynamicObject {
// private:
//     static std::list<TexturePtr> wall_textures;
//     static const TexturePtr* get_rect(sf::Vector2f size, sf::Color color = sf::Color::Transparent);

// public:
//     static Frame get_frame(const sf::RenderWindow& window, float bound_width = 300);

//     explicit Wall(sf::Vector2f size, sf::Vector2f pos = {0, 0}, 
//          sf::Color color = sf::Color::Transparent);
         
//     explicit Wall() : DynamicObject() {}

//     explicit Wall(const sf::Texture& texture, sf::Vector2f pos = {0, 0});

//     Wall(const Wall& other) : DynamicObject(other) {}
    
//     Wall& operator=(const Wall& other) {
//         DynamicObject::operator=(other);
//         return *this;    
//     }

//     bool is_in_right_sector(DynamicObject* obj);
//     bool is_in_left_sector(DynamicObject* obj);
//     bool is_in_upper_sector(DynamicObject* obj);
//     bool is_in_lower_sector(DynamicObject* obj);

//     void on_collide(DynamicObject* obj) override;
//     void on_collide_phys(FramedObject* obj);
// };

struct Frame {
	Wall left;
	Wall right;
	Wall up;
	Wall low;

    std::array<Wall*, 4> iter() {
        return {&left, &right, &up, &low};
    }
};
