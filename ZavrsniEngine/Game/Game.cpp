#if 1
#include "../Source/Engine.h"
#include "Pickups/Ammo.h"

#define NOT_MENU 0

class Game : public engine::Engine
{
	engine::Window* window = createWindow("Game", 800, 600, false);
	graphics::Layer* layer = new graphics::Layer();
	graphics::Font* font = new graphics::Font("Assets/arial.ttf");
	math::Vector2 mousePosition;
	bool inGame = false;

	void init()
	{
		window->toggleCursor();

		lam::LevelAssetManager::init(nullptr, layer);
		graphics::TextureManager::add(new graphics::Texture("Assets/city.png"), "menuBackground");
		graphics::TextureManager::add(new graphics::Texture("Assets/Newgame.png"), "menuNewgame");
		graphics::TextureManager::add(new graphics::Texture("Assets/Quit2.png"), "menuQuit");
		graphics::TextureManager::add(new graphics::Texture("Assets/Score.png"), "menuScore");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor4.png"), "menuCursor");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor5.png"), "menuCursor2");
		lam::LevelAssetManager::add(new graphics::Label("0", -14.0f, 8.0f, 0xff00ff00, 0.3f, font, 4), "FPS");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 32.0f, 18.0f, graphics::TextureManager::get("menuBackground"), 0), "background");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 3.0f, 8.0f, 2.0f, graphics::TextureManager::get("menuNewgame"), 1), "menuNewGame");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 8.0f, 2.0f, graphics::TextureManager::get("menuScore"), 2), "menuScore");
		lam::LevelAssetManager::add(new objects::GameObject(new graphics::Sprite(0.0f, -3.0f, 6.0f, 2.0f, graphics::TextureManager::get("menuQuit"), 3),0), "menuQuit");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 1.5f, 1.5f, graphics::TextureManager::get("menuCursor"), 100), "menuCursor");


		//lam::LevelAssetManager::add(new GameObject(&Sprite(9.0f, 5.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 300), "Planet");

		lam::LevelAssetManager::addToLayer(layer);
	}
	// jednom svake sekunde
	void tick() 
	{
		lam::LevelAssetManager::getLabel("FPS")->setText(std::to_string(_FPS));
	}
	// 60 puta u sekundi
	void update() {}
	// Svaki loop !mora se implementirati!
	void render() 
	{
			lam::LevelAssetManager::processBegin(*window);

			window->getMousePosition(mousePosition);
			graphics::Sprite* cursor = lam::LevelAssetManager::getSprite("menuCursor");

			cursor->setPosition(mousePosition);
			if (window->getMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
				cursor->swapTexture(graphics::TextureManager::get("menuCursor2"));
			else
				cursor->swapTexture(graphics::TextureManager::get("menuCursor2"));

			layer->render();

			objects::GameObject* quit = lam::LevelAssetManager::getGameObject("menuQuit");

			mousePosition.x -= cursor->getSize().x / 2;
			mousePosition.y += cursor->getSize().y / 2;
			if (quit->isHit(mousePosition))
			{
				quit->getSprite()->setColor(0xff8a8a8a);
				if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
				{
					window->close();
				}
			}
			else
				quit->getSprite()->setColor(0xffffffff);

			lam::LevelAssetManager::processEnd(*window);
	};

public:
	~Game() {
		delete layer;
		delete font;
	}
};

int main()
{
	Game game;
	game.start();
	lam::LevelAssetManager::clean();
	graphics::TextureManager::clean();
	return 0;
}
#endif