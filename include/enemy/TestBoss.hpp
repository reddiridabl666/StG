#pragma once

#include "Boss.hpp"

class TestBoss : public AudibleAnimatedBoss {
protected:
    struct BallBounce : public Phase {
        BallBounce(Boss*);
        void update(float) override;
        void shoot() override;
    };

    struct ChessHoming : public Phase {
        ChessHoming(Boss*);
        void update(float) override;
        void shoot() override;
    };
    
    struct Circular_1 : public Phase {
        Circular_1(Boss*);
        void update(float) override;
        void shoot() override;
    };

    struct CircularRotating : public Phase {
        CircularRotating(Boss*);
        void update(float) override;
        void shoot() override;
    };

    struct CircularGrouped : public Phase {
        CircularGrouped(Boss*);
        void update(float) override;
        void shoot() override;
    };

    struct Chaos : public Phase {
        Chaos(Boss*);
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
          Boss(texture, pos, hitbox_size, 0, layer),
          AudibleAnimatedBoss(texture, pos, hitbox_size, 0, layer)
#ifdef DEBUG
        , time_(),
        shot_num_log("Shot num: ", shot_num_, {50, 125}),
        time_log("Shot interval: ", time_, {50, 200})
#endif  
    {
        init_sprites(Resources::sprite_sheets["boss1"]);
        setAnimation(sprites_["idle"]);
        changePhase(new Circular_1(this));
        // changePhase(new Chaos(this));
        phase_max_ = 6;
    }

    void init_sprites(sf::Image sprite_sheet) override {
        sprites_["idle"] = load_row(sprite_sheet, 4, {0, 11}, {64, 40});
        sprites_["death"] = load_row(Resources::sprite_sheets["explosion"], 8, {0, 12}, {64, 40});
    }

    void shoot();
    void update(float deltaTime) override;

    friend TestBoss::BallBounce;
};
