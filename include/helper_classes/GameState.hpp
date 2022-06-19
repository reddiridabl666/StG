#pragma once

#include "GameObject.h"
#include "Controls.hpp"
#include <memory>

class Player;
class Window;
struct Frame;

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

    static void init(const Window* window, const Frame& frame, float offset = 100);
    
    static void setPlayerTracking(std::weak_ptr<Player>& player);

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
