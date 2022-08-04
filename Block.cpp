#include "Block.h"

Block::Block(sf::Vector2f pos, sf::Vector2f size, float downSpeed, float horizationalSpeed, const sf::Texture& texture)
{
	this->bounds = std::make_unique<sf::RectangleShape>();
	this->downSpeed = downSpeed;
	this->horizationalSpeed = horizationalSpeed;

	bounds->setPosition(pos);
	bounds->setSize(size);
	bounds->setTexture(&texture);

	LOG("Created Block");
}

Block::~Block()
{
	LOG("Deleted Block");
}

void Block::tick(const float& dt)
{
	bounds->move(sf::Vector2f(horizationalSpeed * dt, downSpeed * dt));
    remove = checkIfOutOfBounds();
}

void Block::render(sf::RenderTarget& target)
{
	target.draw(*this->bounds);
}

bool Block::checkIfOutOfBounds()
{
	const sf::Vector2f& pos = bounds.get()->getPosition();
	const sf::Vector2f& size = bounds.get()->getSize();

	//if it is moving left
	if (horizationalSpeed < 0) {
		//if moved off the screen
		if (pos.x + size.x < 0)
			return true;
		else
			return false;
	} //if moving right
	else if (horizationalSpeed > 0) {
		if (pos.x > 800)
			return true;
		else
			return false;
	}
	if (pos.y > 800)
		return true;
	else
		return false;
}