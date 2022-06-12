#pragma once

#include "Wall.hpp"
#include "Window.h"
#include "Text.hpp"
#include "Player.hpp"
#include "Resources.hpp"

#include <memory>

class GameState {
private:
    GameState() {}

    static GameState state;

    sf::FloatRect bounds_ = {0, 0, 0, 0};

    std::weak_ptr<Player> player_;
    const Window* window_ = nullptr;

public:
    GameState(const GameState&) = delete;   
    GameState& operator=(const GameState&) = delete;

    static void init(std::weak_ptr<Player> player, const Window* window, const Frame& frame, float offset = 100) {
        state.player_ = player;
        state.window_ = window;

        state.bounds_ = {frame.left.getPosition().x - offset, frame.up.getPosition().y - offset,
                         frame.right.getPosition().x + offset - state.bounds_.left,
                         frame.low.getPosition().y + offset - state.bounds_.top};
                         
    }

	static sf::Vector2f getPlayerPos() {
		return player() ? player()->getPosition() : sf::Vector2f();
	}

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
