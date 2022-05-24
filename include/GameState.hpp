#pragma once

#include "Wall.hpp"
#include "Text.hpp"
#include "Player.hpp"
#include "Resources.hpp"

class GameState {
private:
    GameState() {};
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    static GameState state;

    sf::FloatRect bounds_ = {0, 0, 0, 0};
    sf::Vector2f player_pos_ = {0, 0};

    Player* player_ = nullptr;
    Text* hp;

public:
    static void update() {
        if (state.player_) {
            state.player_pos_ = state.player_->getPosition();

            if (state.player_->HP() <= 0) {
                delete state.player_;
                state.player_ = nullptr;
            }
        }
        state.hp->setString(state.player_ ? std::string("Health: ") + 
                                std::to_string(state.player_->HP()) : "You died");
    }

    static void init(Player* player, const Frame& frame, float offset = 100) {
        state.player_ = player;

        state.hp = new Text(std::to_string(player->HP()), 
                      Resources::fonts["ARIAL"], 48, {1650, 150});

        state.bounds_.left = frame.left.getPosition().x - offset;
        state.bounds_.top = frame.up.getPosition().y - offset;
        state.bounds_.width = frame.right.getPosition().x + offset - state.bounds_.left;
        state.bounds_.height = frame.low.getPosition().y + offset - state.bounds_.top;
    }

	static sf::Vector2f getPlayerPos() {
		return state.player_pos_;
	}

    static Player* player() {
        return state.player_;
    }

	static bool is_out_of_bounds(GameObject* obj) {
		return !state.bounds_.contains(obj->getTransformable()->getPosition());
	}

    ~GameState() {
        if (player_) {
            delete player_;
        }
        if (hp) {
            delete hp;
        }
    }
};

inline GameState GameState::state;
