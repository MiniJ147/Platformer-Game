#pragma once
#include "Block.h"
#include <fstream>
#include <math.h>

//TODO work on player classffg
class Player
{
private:
	std::unique_ptr<Engine::AudioEffect> jumpAudio;
	std::unique_ptr<Engine::AudioEffect> deathAudio;
	std::unique_ptr<Engine::AudioEffect> restartAudio;
	std::unique_ptr<Engine::AudioEffect> pausedAudio;

	bool deathAudioPlayed = false;

	std::vector<std::shared_ptr<Block>>* levelVec;
	std::unique_ptr<sf::RectangleShape> bounds;
	struct EnviromentData {
		float gravity;
		int countCap;
		int count;
		bool useGravity;
	};

	EnviromentData envData;

	sf::Font* font;
	sf::Text* text;
	sf::Text* canJumpText;

	sf::Clock clock = sf::Clock();

public:
	struct PlayerData {
		bool alive;
		float moveSpeed;
		int jumpHeight;
		bool canJump;
		int jumpCoolDownTimer;
		int jumpCoolDownReset;
		int score;
		int highScore;
		bool restart;
		bool paused;
	};
	PlayerData playerInfo;

	Player(sf::Vector2f pos, sf::Vector2f size, const sf::Texture& texture,std::vector<std::shared_ptr<Block>>* levelVec);
	~Player();

	void tick(const float& dt);
	void render(sf::RenderTarget& target);
	void move(float x_dir, float y_dir, const float& dt);

	void checkMovement(const float& dt, int i);
	void jump();
	void addGravity(const float& dt);

	void loadFontAndText();
	void updatePlayerScore();
	void updateGravity(const float& dt);

	int checkCollsisions();
	void checkJump(int currentFrameCollsisions);
	void initAudio();

	void loadStatsFromFile(const std::string& path);
	void loadStatsToFile(const std::string& path);
};

