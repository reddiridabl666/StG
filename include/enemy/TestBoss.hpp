#pragma once

#include "Boss.hpp"

class TestBoss : public AudibleAnimatedBoss {
private:
    Animated::Sprites init_sprites(const sf::Image& sprite_sheet);
    bool flag = false;
    
protected:
    struct BallBounce : public Phase {
        BallBounce(Boss*);
        void update(float) override;
        void shoot() override;
        ~BallBounce() override;
    };

    struct ChessHoming : public Phase {
        ChessHoming(Boss*);
        void shoot() override;
    };
    
    struct Circular_1 : public Phase {
        Circular_1(Boss*);
        void shoot() override;
    };

    struct CircularRotating : public Phase {
        CircularRotating(Boss*);
        void shoot() override;
    };

    struct CircularGrouped : public Phase {
        CircularGrouped(Boss*);
        void shoot() override;
    };

    struct Chaos : public Phase {
        Chaos(Boss*);
        void shoot() override;
    };

    struct StreamsRandom : public Phase {
        StreamsRandom(Boss*);
        void shoot() override;
    };

    struct BoWaP_alike : public Phase {
        BoWaP_alike(Boss*);
        void shoot() override;
    };

#ifdef DEBUG
    float time_;
    Log<size_t> shot_num_log;
    Log<float> time_log;
#endif

public:
    TestBoss(const sf::Vector2f& pos, const HitboxInfo& hitbox_size, Layer layer = Layer::Character) :
          Animated(init_sprites(Resources::sprite_sheets["boss1"])),
          Boss(sprites_["idle"][0], pos, hitbox_size, 0, layer),
          AudibleAnimatedBoss(init_sprites(Resources::sprite_sheets["boss1"]), pos, hitbox_size, 0, layer)
#ifdef DEBUG
        , time_(),
        shot_num_log("Shot num: ", shot_num_, {50, 125}),
        time_log("Shot interval: ", time_, {50, 200})
#endif  
    {
        setAnimation(sprites_["idle"]);
        scale(4.3, 4.3);
        changePhase(std::make_unique<StreamsRandom>(this));
        phase_max_ = 8;
    }

    void hide() override {
        Boss::hide();
        health_bar_.hide();
        phase_left_.hide();
    }

    void show() override {
        Boss::show();
        health_bar_.show();
        phase_left_.show();
    }
    
    void shoot();
    void update(float deltaTime) override;

    friend TestBoss::BallBounce;
};
