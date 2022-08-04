#include "Game.h"

Game::Game(unsigned int w_width, unsigned int w_height, std::string title)
{
	this->window = new sf::RenderWindow(sf::VideoMode(w_width, w_height), title);
	this->gameState = new GameState(*this->window);

	run();
}

Game::~Game()
{
	delete window;
	delete gameState;
}

void Game::run()
{
	while (window->isOpen()) {
		updateSFMLEvents();
		updateDt();
		tick(this->dt);

		//rendering
		window->clear(sf::Color::Black);

		render(*window);
		
		window->display();

		if (gameState->restart) {
			gameState->~GameState();
			this->gameState = new GameState(*window);
		}
	}
}

void Game::updateDt()
{
	this->dt = this->clock.restart().asSeconds();
}

void Game::tick(const float& dt)
{
	gameState->tick(dt);
}

void Game::render(sf::RenderTarget& target)
{
	gameState->render(target);
}

void Game::updateSFMLEvents()
{
	sf::Event e;
	while (window->pollEvent(e)) {
		if (e.type == sf::Event::Closed)
			window->close();
	}
}
