#include "LevelManager.h"

LevelManager::LevelManager(Engine::Assets& assets)
{
	int startingBlocks = 10;

	srand(time(NULL));

	this->assets = &assets;

	for (int i = 0; i < startingBlocks; i++) {
		generateRandomBlock(700);
	}
}

LevelManager::~LevelManager()
{
	LOG(blocks.size());
}

void LevelManager::tick(const float& dt)
{
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->tick(dt);
		if (blocks[i]->remove == true) {
			removeBlock(i);
			generateRandomBlock(1);
		}
	}
}

void LevelManager::render(sf::RenderTarget& target)
{
	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->render(target);
	}
}

void LevelManager::addBlock(std::shared_ptr<Block> block)
{
	blocks.push_back(block);
}

void LevelManager::removeBlock(int i)
{
	blocks.erase(blocks.begin() + i);
}

void LevelManager::generateRandomBlock(int yOffset)
{
	LOG("Generated new block\n");
	int x_pos = rand() % 8;	
	float y_pos = rand() % yOffset;
	float s_width = rand() % 150 + 90;
	float downSpeed = rand() % 140 + 95;
	addBlock(std::make_shared<Block>(sf::Vector2f(x_pos * 100 ,y_pos),sf::Vector2f(s_width,30.f),downSpeed,0.f,*(assets->gameAssets["platform"].texture)));
}
