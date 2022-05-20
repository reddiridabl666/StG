#include "Bullet.hpp"
#include "UpdateFunctions.hpp"
#include "Resources.hpp"

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
                    other.getVelocity(),}) {
    setTag(other.getTag());
}

Bullet& Bullet::operator=(const Bullet& other) {
    clock.restart();
    setUpdateFunc(other.update_);
    DynamicObject::operator=(other);
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

    res["test_circle"] = BulletInfo{&textures["test_circle"], 25,
                          {0, 0}, &gravity, 1, 1};

    res["test_player"] = BulletInfo{&textures["player_bullet"], 
                                sf::Vector2f{32.f, 56.f}, {0, -600}, &delete_when_out_of_bounds, 50, 0};
    return res;
}

std::unordered_map<std::string, BulletInfo> Bullet::BulletTypes = Bullet::getBulletTypes();
