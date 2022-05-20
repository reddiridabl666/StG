#pragma once

#include "Wall.hpp"

class GameState {
  private:
    GameState() {};
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    static GameState state;
  public:
    sf::Vector2f player_pos_ = {0, 0};
	sf::FloatRect bounds_ = {0, 0, 0, 0};

    // static GameState& getState() {
    //     return state;
    // }

    static void update(const sf::Vector2f& player_pos) {
        state.player_pos_ = player_pos;
    }

    static void set_frame(const Frame& frame, float offset = 100) {
        state.bounds_.left = frame.left.getPosition().x - offset;
        state.bounds_.top = frame.up.getPosition().y - offset;
        state.bounds_.width = frame.right.getPosition().x + offset - state.bounds_.left;
        state.bounds_.height = frame.low.getPosition().y + offset - state.bounds_.top;
    }

	static sf::Vector2f getPlayerPos() {
		return state.player_pos_;
	}

	static bool is_out_of_bounds(GameObject* obj) {
		return !state.bounds_.contains(obj->getTransformable()->getPosition());
	}
};

inline GameState GameState::state;
