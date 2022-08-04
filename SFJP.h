#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <unordered_map>
#include <SFML/Audio.hpp>

namespace Engine{
	class UI
	{
	protected:
		std::unique_ptr<sf::RectangleShape> shape;

	public:
		int id;

		virtual void tick(const float& dt) = 0;
		virtual void render(sf::RenderTarget& target) = 0;
		virtual void createSprite(sf::Vector2f pos, sf::Vector2f size, const sf::Texture& texture) = 0;
	};

	class UIButton :
		public UI
	{
	private:
		short int x = 0;

		bool hovering;

		sf::Vector2i* mousePos;
		bool* isPressed;

		sf::Vector2f size;

		std::function<void()> func;
	public:
		UIButton(int id, sf::Vector2f pos, sf::Vector2f size, const sf::Texture& texture, sf::Vector2i& m_p, bool& isPressed, const std::function<void()>& func) {
			this->size = size;
			this->id = id;
			createSprite(pos, size, texture);

			this->mousePos = &m_p;
			this->isPressed = &isPressed;
			this->func = func;
		}

		~UIButton() {

		}

		void tick(const float& dt) {
			//clicking logic
			if (*isPressed && x == 0)
				x = 1;

			if (!*isPressed && x == -1)
				x = 0;

			if (hovering && x == 1) {
				x = -1;
				onClick();
			}

			if (shape.get()->getGlobalBounds().contains((sf::Vector2f)*mousePos)) {
				hovering = true;
				shape->setTextureRect(sf::IntRect(0, size.y, size.x, size.y));
			}
			else {
				hovering = false;
				shape->setTextureRect(sf::IntRect(0, 0, size.x, size.y));
			}
		}

		void render(sf::RenderTarget& target) {
			target.draw(*shape);
		}
		void createSprite(sf::Vector2f pos, sf::Vector2f size, const sf::Texture& texture) {
			this->shape = std::make_unique<sf::RectangleShape>();
			this->shape->setPosition(pos);
			this->shape->setSize(size);
			this->shape->setTexture(&texture);
		}
		void onClick() {
			func();
		}

		bool getHovering() {
			return hovering;
		}
	};

	class UIManager
	{
	private:
		std::vector<std::shared_ptr<UI>> UI_Elements;

	public:
		UIManager() {

		}
		~UIManager() {

		}

		void tick(const float& dt) {
			for (auto u : UI_Elements)
				u->tick(dt);
		}
		void render(sf::RenderTarget& target) {
			for (auto u : UI_Elements)
				u->render(target);
		}
		void addUIElement(std::shared_ptr<UI> U) {
			UI_Elements.push_back(U);
		}
		void removeUIElementIndex(int index) {
			UI_Elements.erase(UI_Elements.begin() + index);
		}
		void removeUIElementID(int ID) {
			bool found = false;
			int i = 0;

			for (auto u : UI_Elements) {
				if (u.get()->id == ID) {
					found = true;
					break;
				}
				else
					i++;
			}

			if (found)
				UI_Elements.erase(UI_Elements.begin() + i);
		}
	};
	
	class Assets
	{
	private:
		struct SpriteData {
			std::string name;
			std::unique_ptr<sf::Texture> texture;
		};

	public:
		std::unordered_map<std::string, SpriteData> gameAssets;

		Assets() {

		}
		virtual ~Assets() {

		}

		void createGameAsset(std::string key, const std::string& path) {
			gameAssets[key] = SpriteData{ key, std::make_unique<sf::Texture>() };
			gameAssets[key].texture->loadFromFile(path);
		}
	};

	class MouseManager
	{
	public:
		sf::RenderWindow& window;

		sf::Vector2i* mousePos;
		bool* leftDown;

		MouseManager(sf::RenderWindow& window) :
		window(window) {

			leftDown = new bool(false);

			init();
		}
		~MouseManager() {
			delete mousePos;
			delete leftDown;
		}

		void tick() {
			*leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

			updateMousePos();
		}
		void init() {
			this->mousePos = new sf::Vector2i(sf::Mouse::getPosition(window));
		}

		inline void updateMousePos() {
			sf::Vector2i p = sf::Mouse::getPosition(window);
			mousePos->x = p.x;
			mousePos->y = p.y;
		}
	};

	class AudioEffect {
	private:
		std::unique_ptr<sf::SoundBuffer> soundBuffer;
		std::unique_ptr<sf::Sound> sound;

	public:
		AudioEffect(const std::string& path) {
			soundBuffer = std::make_unique<sf::SoundBuffer>();
			soundBuffer->loadFromFile(path);

			sound = std::make_unique<sf::Sound>();
			sound->setBuffer(*soundBuffer);
		}

		~AudioEffect() {

		}

		void play() {
			sound->play();
		}
	};
}