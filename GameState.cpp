#include "GameState.h"

GameState::GameState(sf::RenderWindow& window)
{
	this->assets = std::make_unique<Engine::Assets>();
	initAssets();

	this->levelManager = std::make_unique<LevelManager>(*assets);
	levelManager->addBlock(std::make_unique<Block>(sf::Vector2f(150.f, 500.f), sf::Vector2f(500.f, 32.f), 15.f, 0.f, *(assets->gameAssets["platform"].texture)));

	this->player = std::make_unique<Player>(sf::Vector2f(100.f, 200.f), sf::Vector2f(48.f, 48.f), *(assets->gameAssets["player"].texture), &levelManager->blocks);
	
	this->uiManager = new Engine::UIManager();
	this->mouseManager = new Engine::MouseManager(window);

	uiManager->addUIElement(std::make_shared<Engine::UIButton>(0, sf::Vector2f(200.f, 200.f), sf::Vector2f(400.f, 200.f), *(assets->gameAssets["start button"].texture), *mouseManager->mousePos, *mouseManager->leftDown, [=]() {start = true; }));

	createBackGroundTexture();
}

GameState::~GameState()
{
	delete backGroundTexture;
	delete shape;
	delete uiManager;
	delete mouseManager;
}

void GameState::tick(const float& dt)
{
	if (!start) {
		mouseManager->tick();
		uiManager->tick(dt);
	}

	if (!player->playerInfo.paused && start)
	{
		levelManager->tick(dt);
		player->tick(dt);

		if (player->playerInfo.restart)
			restart = true;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			player->playerInfo.paused = false;
	}
}

void GameState::render(sf::RenderTarget& target)
{
	target.draw(*shape);

	levelManager->render(target);
	player->render(target);

	if(!start)
		uiManager->render(target);
}

void GameState::initAssets()
{
	assets->createGameAsset("platform","resources/img/platform.png");
	assets->createGameAsset("player", "resources/img/player.png");
	assets->createGameAsset("start button", "resources/img/startButton.png");
}

void GameState::createBackGroundTexture()
{
	backGroundTexture = new sf::Texture();
	backGroundTexture->loadFromFile("resources/img/background.png");

	shape = new sf::RectangleShape();
	shape->setSize(sf::Vector2f(800.f, 800.f));
	shape->setTexture(backGroundTexture);
}

