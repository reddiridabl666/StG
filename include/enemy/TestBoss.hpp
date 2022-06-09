#pragma once

#include "Boss.hpp"

class TestBoss : public AnimatedBoss {
protected:
    struct BallBounce : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };

    struct ChessHoming : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };
    
    struct Circular_1 : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };

    struct Circular_2 : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };

    struct CircularGrouped : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };

    struct Chaos : public Phase {
        using Phase::Phase;
        void update(float) override;
        void shoot() override;
    };

#ifdef DEBUG
    float time_;
    Log<int> shot_num_log;
    Log<float> time_log;
#endif

public:
    TestBoss(const sf::Texture& texture, const sf::Vector2f& pos, 
          const HitboxInfo& hitbox_size, Layer layer = Layer::Character) : 
        AnimatedBoss(texture, pos, hitbox_size, 0, layer)
#ifdef DEBUG
        , time_(),
        shot_num_log("Shot num: ", shot_num_, {50, 125}),
        time_log("Shot interval: ", time_, {50, 200})
#endif  
    {
        init_sprites(Resources::sprite_sheets["boss1"]);
        setAnimation(sprites_["idle"]);
        // changePhase(new Circular_1(this, 6000));
        changePhase(new Chaos(this, 6000));
        phase_max_ = 6;
    }

    void init_sprites(sf::Image sprite_sheet) override {
        sprites_["idle"] = load_row(sprite_sheet, 4, {0, 11}, {64, 40});
    }

    // void shoot(/* std::string name */);
    void update(float deltaTime) override;

    friend TestBoss::BallBounce;
};
