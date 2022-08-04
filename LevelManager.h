#pragma once
#include "Block.h"
#include <stdio.h>
#include <time.h>

class LevelManager
{
private:
	Engine::Assets* assets;
public:
	std::vector<std::shared_ptr<Block>> blocks;

	LevelManager(Engine::Assets& assets);
	~LevelManager();

	void tick(const float& dt);
	void render(sf::RenderTarget& target);
	void addBlock(std::shared_ptr<Block> block);
	void removeBlock(int i);
	void generateRandomBlock(int yOffset);
};

