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
    sf::Vector2f player_pos_ = {0, 0};

    Player* player_ = nullptr;
    Window* window_ = nullptr;
    std::unique_ptr<Text> hp_;

public:
    // friend class Game;

    GameState(const GameState&) = delete;   
    GameState& operator=(const GameState&) = delete;

    static void update(Player* player) {
        // state.player_ = player;
        if (!player) {
            state.player_ = nullptr;
        }
        if (state.player_) {
            state.player_pos_ = state.player_->getPosition();
        }
        state.hp_->setString(state.player_ ? std::string("Health: ") + 
                                std::to_string(state.player_->HP()) : "You died");
    }

    static void init(Player* player, Window* window, const Frame& frame, float offset = 100) {
        state.player_ = player;
        state.window_ = window;

        state.hp_ = std::make_unique<Text>(std::to_string(player->HP()), 
                                    Resources::fonts["ARIAL"], 
                                    48, sf::Vector2f{1650, 150});

        state.bounds_ = {frame.left.getPosition().x - offset, frame.up.getPosition().y - offset,
                         frame.right.getPosition().x + offset - state.bounds_.left,
                         frame.low.getPosition().y + offset - state.bounds_.top};
        // state.bounds_.left = frame.left.getPosition().x - offset;
        // state.bounds_.top = frame.up.getPosition().y - offset;
        // state.bounds_.width = frame.right.getPosition().x + offset - state.bounds_.left;
        // state.bounds_.height = frame.low.getPosition().y + offset - state.bounds_.top;
    }

	static sf::Vector2f getPlayerPos() {
		return state.player_pos_;
	}

    static const Window* window() {
        return state.window_;
    }

    static Player* player() {
        return state.player_;
    }

	static bool is_out_of_bounds(GameObject* obj) {
		return !state.bounds_.contains(obj->getTransformable()->getPosition());
	}

    ~GameState() {}
};
