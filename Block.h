#pragma once
#include "SFJP.h"
#include "DEBUG_TOOLS.h"
#include <iostream>

class Block
{
public:
	bool remove = false;

	float downSpeed;
	float horizationalSpeed;

	std::unique_ptr<sf::RectangleShape> bounds;

	Block(sf::Vector2f pos, sf::Vector2f size, float downSpeed, float horizationalSpeed, const sf::Texture& texture);
	~Block();

	void tick(const float& dt);
	void render(sf::RenderTarget& target);

	bool checkIfOutOfBounds();
};

