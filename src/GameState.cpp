#include "GameState.hpp"

#include "Wall.hpp"
#include "Window.h"
#include "Player.hpp"

GameState GameState::state;

void GameState::init(const Window* window, const Frame& frame, float offset) {
    state.window_ = window;

    state.bounds_ = {frame.left.getPosition().x - offset, 
                     frame.up.getPosition().y - offset,
                     frame.right.getPosition().x + offset - state.bounds_.left,
                     frame.low.getPosition().y + offset - state.bounds_.top};           
}

void GameState::setPlayerTracking(std::weak_ptr<Player>& player) {
    state.player_ = player;
}

sf::Vector2f GameState::getPlayerPos() {
    return player() ? player()->getPosition() : sf::Vector2f();
}

