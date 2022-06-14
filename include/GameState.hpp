#pragma once

#include "GameObject.h"
#include "Controls.hpp"
#include <memory>

class Player;
class Window;
struct Frame;

class GameState {
    struct Settings_ {
        sf::Uint16 volume = 100;

        Key::Key k_shoot = Key::Key::Space;
        Key::Key k_slow = Key::Key::LShift;

        Gamepad::Button g_shoot = Gamepad::A;
        Gamepad::Button g_slow = Gamepad::RB;

        void volumeUp(sf::Uint8 vol = 1);
        void volumeDown(sf::Uint8 vol = 1);

        void setShoot(Key::Key k, Gamepad::Button g);
        void setSlow(Key::Key k, Gamepad::Button g);
    };
private:
    GameState() {}

    static GameState state;

    sf::FloatRect bounds_ = {0, 0, 0, 0};

    std::weak_ptr<Player> player_;
    const Window* window_ = nullptr;
    Settings_ settings;

public:
    GameState(const GameState&) = delete;   
    GameState& operator=(const GameState&) = delete;

    static void init(std::weak_ptr<Player> player, const Window* window, const Frame& frame, float offset = 100);

    static Settings_& Settings() {
        return state.settings;
    }

	static sf::Vector2f getPlayerPos();

    static const Window* window() {
        return state.window_;
    }

    static const std::shared_ptr<Player> player() {
        return state.player_.expired() ? nullptr : state.player_.lock();
    }

	static bool is_out_of_bounds(GameObject* obj) {
		return !state.bounds_.contains(obj->getTransformable()->getPosition());
	}

    ~GameState() {}
};
