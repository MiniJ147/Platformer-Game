#include "Player.h"


Player::Player(sf::Vector2f pos, sf::Vector2f size, const sf::Texture& texture, std::vector<std::shared_ptr<Block>>* levelVec)
{
	playerInfo = PlayerData{true,350.f, 256,true,0, 550, 0};
	envData = EnviromentData{ 1.5f,14,0,false};

	loadStatsFromFile("resources/stats.txt");

	this->levelVec = levelVec;

	bounds = std::make_unique<sf::RectangleShape>();

	bounds->setSize(size);
	bounds->setPosition(pos);
	bounds->setTexture(&texture);
	
	loadFontAndText();
	initAudio();

	restartAudio->play();
}

Player::~Player()
{
	loadStatsToFile("resources/stats.txt");

	delete font;
	delete text;
	delete canJumpText;
}

void Player::tick(const float& dt)
{
	if (playerInfo.alive) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			pausedAudio->play();
			text->setString("Paused\nPress E to unpause");
			text->setCharacterSize(30);
			playerInfo.paused = true;
			return;
		}

		int currentFrameCollsisions = checkCollsisions();

		switch (currentFrameCollsisions) {
		case 1:
			LOG("Down Collision");
			break;
		case 2:
			LOG("Up Collision");
			break;
		case 3:
			LOG("Right Collision");
			break;
		}

		envData.useGravity = currentFrameCollsisions == 1 ? false : true;
		
		checkJump(currentFrameCollsisions);

		//check if fell into out of bounds then die
		updateGravity(dt);

		if (bounds.get()->getPosition().y > 799)
			playerInfo.alive = false;

		checkMovement(dt, currentFrameCollsisions);
		updatePlayerScore();
	}
	else {
		if (!deathAudioPlayed) {
			deathAudio->play();
			deathAudioPlayed = true;
		}

		text->setString("Current Score: " + std::to_string(playerInfo.score) + "\nHigh Score: " + std::to_string(playerInfo.highScore) + "\n\n\nPress R to Restart");
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			playerInfo.restart = true;
		}
	}
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(*bounds);
	target.draw(*text);
	target.draw(*canJumpText);
}

void Player::move(float x_dir, float y_dir, const float& dt)
{
	if(playerInfo.alive==true)
		bounds->move(x_dir * playerInfo.moveSpeed * dt, y_dir * playerInfo.jumpHeight* dt);
}

void Player::checkMovement(const float& dt, int t)
{
	//checking movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && playerInfo.canJump) {
		jump();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && t!=4 && bounds->getPosition().x > -8)
		move(-1.0f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && t!=3 && bounds->getPosition().x + bounds->getSize().x <= 808)
		move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && t!=1)
		move(0.f, 1.f, dt);
}

void Player::jump()
{
	playerInfo.canJump = false;
	jumpAudio->play();
	canJumpText->setFillColor(sf::Color::Red);
	playerInfo.jumpCoolDownTimer = playerInfo.jumpCoolDownReset;

	for (int i = 0; i < playerInfo.jumpHeight; i++)
		if (checkCollsisions() == 2 || bounds->getPosition().y <= 1)
			break;
		else
			bounds->move(0.f, -1.f);
}

void Player::addGravity(const float& dt)
{
	if (envData.count < envData.countCap)
		envData.count++;

	bounds->move(0.f, pow(envData.gravity, envData.count)*dt);
}

void Player::loadFontAndText()
{
	this->font = new sf::Font();
	this->text = new sf::Text();

	this->canJumpText = new sf::Text();

	font->loadFromFile("resources/font/m42.TTF");
	text->setFont(*font);
	text->setCharacterSize(20);

	canJumpText->setFont(*font);
	canJumpText->setCharacterSize(20);

	canJumpText->setString("JUMP");
	canJumpText->setPosition(sf::Vector2f(0.f, 150.f));

	clock.restart().asSeconds();
}

void Player::updatePlayerScore()
{
	text->setCharacterSize(20);
	text->setString("Current Score: " + std::to_string(playerInfo.score) + "\nHigh Score: " + std::to_string(playerInfo.highScore) + "\n\nPress Q to Pause");

	if (playerInfo.alive)
		if (clock.getElapsedTime().asSeconds() >= 0.5f) {
			playerInfo.score += 1;
			clock.restart().asSeconds();
		}
}

void Player::updateGravity(const float& dt)
{
	if (envData.useGravity) {
		addGravity(dt);
	}
}

int Player::checkCollsisions()
{
	for (int i = 0; i < levelVec->size(); i++)
	{
		Block& b = *levelVec->at(i);

		if (bounds->getGlobalBounds().intersects(b.bounds->getGlobalBounds())) {
			sf::Vector2f pos = bounds->getPosition();
			sf::Vector2f b_pos = b.bounds->getPosition();

			if (pos.x + bounds->getSize().x >= b_pos.x-2 && pos.x + bounds->getSize().x <= b_pos.x + 1 && pos.y >= b_pos.y - b.bounds->getSize().y)
				return 3;
			if (pos.x <= b_pos.x + b.bounds->getSize().x + 1 && pos.x >= b_pos.x + b.bounds->getSize().x - 2 && pos.y >= b_pos.y - b.bounds->getSize().y)
				return 4;
			if ((pos.x + bounds->getSize().x) >= b_pos.x && pos.x <= b_pos.x + b.bounds->getSize().x && pos.y + b.bounds->getSize().y <= b_pos.y-1)
				return 1;
			//upward collision
			if ((pos.x + bounds->getSize().x) >= b_pos.x && pos.x <= b_pos.x + b.bounds->getSize().x && pos.y >= b_pos.y)
				return 2;
		}
	}
	return 0;
}

void Player::checkJump(int currentFrameCollsisions)
{
	if (playerInfo.jumpCoolDownTimer == 0) {
		if (currentFrameCollsisions == 1 || currentFrameCollsisions == 3 || currentFrameCollsisions == 4)
		{
			playerInfo.canJump = true;
			canJumpText->setFillColor(sf::Color::Green);
		}
	}
	else {
		playerInfo.jumpCoolDownTimer--;
		LOG(playerInfo.jumpCoolDownTimer);
	}
}

void Player::initAudio()
{
	jumpAudio = std::make_unique<Engine::AudioEffect>("resources/audio/jump.wav");
	deathAudio = std::make_unique<Engine::AudioEffect>("resources/audio/death.wav");
	restartAudio = std::make_unique < Engine::AudioEffect>("resources/audio/restartButtonPushed.wav");
	pausedAudio = std::make_unique<Engine::AudioEffect>("resources/audio/pause.wav");
}

void Player::loadStatsFromFile(const std::string& path)
{
	LOG("STARTING LOAD FROM FILE FUNCTION----------------|");
	std::ifstream file(path);
	std::string line;

	std::getline(file, line);
	playerInfo.highScore = std::stoi(line);
	LOG("HIGH SCORE: " << line);
	LOG("ENDING LOAD FROM FILE FUNCTION------------------|");

	file.close();
}

void Player::loadStatsToFile(const std::string& path)
{
	LOG("STARTING SAVE TO FILE FUNCTION------------------|");
	std::ofstream file("resources/stats.txt");

	if (playerInfo.score > playerInfo.highScore)
		file << std::to_string(playerInfo.score) << "\n";
	else
		file << std::to_string(playerInfo.highScore) << "\n";

	file.close();
	LOG("ENDING SAVE TO FILE FUNCTION--------------------|");
}
