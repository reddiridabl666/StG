#include "Bullet.hpp"
#include "UpdateFunctions.hpp"

Bullet::Bullet(BulletInfo info, Layer layer) : Bullet(layer) {
    if (info.texture) {
        setTexture(*info.texture);
    }
    if (info.update) {
        setUpdateFunc(*info.update);
    }
    if (auto size = std::get_if<sf::Vector2f>(&info.hitbox_info)) {
        hitbox_ = new RectHitbox(*size);
    } else if (auto size = std::get_if<float>(&info.hitbox_info)) {
        hitbox_= new CircleHitbox(*size);
    }
    setVelocity(info.velocity);
    setMass(info.mass);
}

Bullet::Bullet(const Bullet& other) : 
    Bullet(BulletInfo{other.getTexture(), 
                    dynamic_cast<const CircleHitbox*>(other.getHitbox()) ? 
                    HitboxInfo(other.getHitbox()->getHalfSize().x) : 
                    HitboxInfo(other.getHitbox()->getSize()),
                    other.getVelocity()}) {}

Bullet& Bullet::operator=(const Bullet& other) {
    clock.restart();
    setUpdateFunc(other.update_);
    DynamicObject::operator=(other);
    return *this;
}


std::unordered_map<std::string, sf::Texture> Bullet::textures;

void Bullet::init_textures() {
    sf::RenderTexture texture;

    texture.create(30, 30);
    texture.clear(sf::Color::Transparent);
    textures["test_circle"] = texture.getTexture();

    texture.create(20, 40);
    texture.clear(sf::Color::Transparent);
    textures["player_bullet"] = texture.getTexture();
}

std::unordered_map<std::string, BulletInfo> Bullet::getBulletTypes() {
    init_textures();
    std::unordered_map<std::string, BulletInfo> res;

    res["test_circle"] = BulletInfo{&textures["test_circle"], textures["test_circle"].getSize().x, 
                          {0, 0}, &delete_when_out_of_bounds, 1};

    res["test_player"] = BulletInfo{&textures["player_bullet"], 
                                static_cast<sf::Vector2f>(textures["player_bullet"].getSize()), 
                                {0, -600}, &delete_when_out_of_bounds};
    return res;
}
