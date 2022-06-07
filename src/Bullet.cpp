#include "Bullet.hpp"
#include "UpdateFunctions.hpp"
#include "Resources.hpp"

Bullet::Bullet(Bullet::Info info, Layer layer) : Bullet(layer) {
    if (info.texture) {
        setTexture(*info.texture);
    }

    float* n;
    if ((n = std::get_if<float>(&info.phys_info)) && *n) {
        frame_hitbox_ = Hitbox::getFrameHitbox(info.phys_info);
    } else {
        frame_hitbox_ = Hitbox::getFrameHitbox(getSize());
    }

    // if (info.update) {
    //     setUpdateFunc(*info.update);
    // }
    setUpdateFunc(info.update);

    hitbox_ = Hitbox::getHitbox(info.hitbox_info);

    setDamage(info.damage);
    setVelocity(info.velocity);
    setMass(info.mass);
}

Bullet::Bullet(const Bullet& other) : 
    Bullet(Bullet::Info{other.getTexture(), other.getHitbox()->getInfo(), 
                      other.getVelocity(), other.update_, other.damage_, 
                      other.mass_, other.frame_hitbox_->getInfo()}) {
    setTag(other.getTag());
}

Bullet& Bullet::operator=(const Bullet& other) {
    clock.restart();
    setUpdateFunc(other.update_);
    setDamage(other.damage_);
    /* DynamicObject */FramedObject::operator=(other);
    return *this;
}

std::unordered_map<BulletType, sf::Texture> Bullet::textures;

std::unordered_map<BulletType, sf::Texture> Bullet::getBulletTextures() {
    std::unordered_map<BulletType, sf::Texture> res;
    // sf::RenderTexture texture;
    sf::Image &bullets = Resources::sprite_sheets["bullets"];

    res[BulletType::BigCircle_Red].loadFromImage(bullets, {6, 466, 62, 62});
    res[BulletType::Talisman_RB].loadFromImage(bullets, {23, 119, 12, 14});
    res[BulletType::Player].loadFromImage(bullets, {233, 71, 8, 14});
    res[BulletType::Circle_Red].loadFromImage(bullets, {40, 308, 28, 28});

    return res;
}

std::unordered_map<BulletType, Bullet::Info> Bullet::getBulletTypes() {
    textures = getBulletTextures();
    std::unordered_map<BulletType, Bullet::Info> res;

    auto type = BulletType::BigCircle_Red;
    res[type] = Bullet::Info{&textures[type], 36, {0, 0}, gravity + delete_when_out_of_bounds, 
                             1, 1, textures[type].getSize().x / 2};

    type = BulletType::Player;
    res[type] = Bullet::Info{&textures[type], sf::Vector2f{8.f, 14.f} * 4.f, {0, -600}, 
                             delete_when_out_of_bounds, 25, 0, sf::Vector2f{100, 100}};
    
    type = BulletType::Talisman_RB;
    res[type] = Bullet::Info{&textures[type], sf::Vector2f{12, 14} * 2.5f, {0, 500}, delete_when_out_of_bounds};

    type = BulletType::Circle_Red;
    res[type] = Bullet::Info{&textures[type], 30, {0, 400}, delete_when_out_of_bounds, 1, 0, textures[type].getSize().x / 2};
    
    return res;
}

std::unordered_map<BulletType, Bullet::Info> Bullet::Types = Bullet::getBulletTypes();
