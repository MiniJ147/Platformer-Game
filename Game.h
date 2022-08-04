#pragma once
#include "GameState.h"

class Game
{
private:
	sf::RenderWindow* window;
	float dt;
	sf::Clock clock;

	GameState* gameState;

public:
	Game(unsigned int w_width, unsigned int w_height, std::string title);
	~Game();

	void run();
	void updateDt();
	void tick(const float& dt);
	void render(sf::RenderTarget& target);

	//unimportant functions
	void updateSFMLEvents();
};

