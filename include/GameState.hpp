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

    static GameState& getState() {
        return state;
    }

    void update(const sf::Vector2f& player_pos) {
        player_pos_ = player_pos;
    }

    void set_frame(const Frame& frame, float offset = 100) {
        bounds_.left = frame.left.getPosition().x - offset;
        bounds_.top = frame.up.getPosition().y - offset;
        bounds_.width = frame.right.getPosition().x + offset - bounds_.left;
        bounds_.height = frame.low.getPosition().y + offset - bounds_.top;
    }

	sf::Vector2f getPlayerPos() {
		return player_pos_;
	}

	bool is_out_of_bounds(GameObject* obj) {
		return !bounds_.contains(obj->getTransformable()->getPosition());
	}
};

inline GameState GameState::state;
