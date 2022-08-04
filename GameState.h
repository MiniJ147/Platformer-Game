#pragma once
#include "LevelManager.h"
#include "Player.h"

class GameState
{
private:
	std::unique_ptr<Engine::Assets> assets;

	std::unique_ptr<LevelManager> levelManager;
	std::unique_ptr<Player> player;

	sf::Texture* backGroundTexture;
	sf::RectangleShape* shape;

	Engine::MouseManager* mouseManager;
	Engine::UIManager* uiManager;

	bool start = true;

public:
	bool restart;

	GameState(sf::RenderWindow& window);
	~GameState();

	void tick(const float& dt);
	void render(sf::RenderTarget& target);

	void initAssets();
	void createBackGroundTexture();
};

