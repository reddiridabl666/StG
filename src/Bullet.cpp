#include "Bullet.hpp"
#include "UpdateFunctions.hpp"
#include "Resources.hpp"

Bullet::Bullet(BulletInfo info, Layer layer) : Bullet(layer) {
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
    Bullet(BulletInfo{other.getTexture(), other.getHitbox()->getInfo(), 
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

std::unordered_map<std::string, sf::Texture> Bullet::textures;

std::unordered_map<std::string, sf::Texture> Bullet::getBulletTextures() {
    std::unordered_map<std::string, sf::Texture> res;
    // sf::RenderTexture texture;
    sf::Image &bullets = Resources::sprite_sheets["bullets"];

    res["test_circle"].loadFromImage(bullets, {198, 466, 62, 62});

    res["player_bullet"].loadFromImage(bullets, {233, 71, 8, 14});
    return res;
}

std::unordered_map<std::string, BulletInfo> Bullet::getBulletTypes() {
    textures = getBulletTextures();
    std::unordered_map<std::string, BulletInfo> res;

    res.insert({"test_circle", BulletInfo{&textures["test_circle"], 36,
                          {0, 0}, gravity + delete_when_out_of_bounds, 
                          1, 1, textures["test_circle"].getSize().x / 2}});

    res.insert({"test_player", BulletInfo{&textures["player_bullet"], 
                                sf::Vector2f{8.f, 14.f} * 4.f, {0, -600}, 
                                delete_when_out_of_bounds, 25, 0, sf::Vector2f{100, 100}}});
    return res;
}

std::unordered_map<std::string, BulletInfo> Bullet::BulletTypes = Bullet::getBulletTypes();
