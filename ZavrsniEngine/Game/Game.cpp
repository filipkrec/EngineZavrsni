#if 0
#include "../Source/Engine.h"
#include "Ammo.h"

class Game : public engine::Engine
{
	engine::Window* window = createWindow("Game", 800, 600, false);
	graphics::Layer* menu = new graphics::Layer(math::Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	graphics::Font* font = new graphics::Font("Assets/arial.ttf", 24);
	double x;
	double y;
	void init()
	{
		window->toggleCursor();
		font->setScale(800.0f / 32.0f, 600.0f / 18.0f);

		LevelAssetManager::init(new physics::Player());

		graphics::TextureManager::add(new graphics::Texture("Assets/city.png"), "menuBackground");
		graphics::TextureManager::add(new graphics::Texture("Assets/Newgame.png"), "menuNewgame");
		graphics::TextureManager::add(new graphics::Texture("Assets/Quit2.png"), "menuQuit");
		graphics::TextureManager::add(new graphics::Texture("Assets/Score.png"), "menuScore");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor4.png"), "menuCursor");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor5.png"), "menuCursor2");
		LevelAssetManager::add(new graphics::Label("0", -15.0f, 8.0f, 0xff00ff00, font, 4), "FPS");
		LevelAssetManager::add(new graphics::Sprite(-16.0f, -9.0f, 32.0f, 18.0f, graphics::TextureManager::get("menuBackground"), 0), "background");
		LevelAssetManager::add(new graphics::Sprite(-4.0f, 0.0f, 8.0f, 8.0f, graphics::TextureManager::get("menuNewgame"), 1), "menuNewGame");
		LevelAssetManager::add(new graphics::Sprite(-4.0f, -3.0f, 8.0f, 8.0f, graphics::TextureManager::get("menuScore"), 2), "menuScore");
		LevelAssetManager::add(new physics::GameObject(graphics::Sprite(-3.0f, -5.0f, 6.0f, 2.0f, graphics::TextureManager::get("menuQuit"), 3),0), "menuQuit");
		LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 1.5f, 1.5f, graphics::TextureManager::get("menuCursor"), 100), "menuCursor");

		LevelAssetManager::addToLayer(menu);
	}
	// jednom svake sekunde
	void tick() 
	{
		LevelAssetManager::getLabel("FPS")->setText(std::to_string(_FPS));
	}
	// 60 puta u sekundi
	void update() {}
	// Svaki loop !mora se implementirati!
	void render() 
	{
		window->getMousePosition(x, y);
		x = (x / window->getWidth()) * 32 - 16;
		y = -((y / window->getHeight()) * 18 - 9);
		graphics::Sprite* cursor = LevelAssetManager::getSprite("menuCursor");

		cursor->setPosition(math::Vector2(x - 0.3f ,y-1.0f));
		if (window->getMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			cursor->swapTexture(graphics::TextureManager::get("menuCursor2"));
		else
			cursor->swapTexture(graphics::TextureManager::get("menuCursor2"));

		menu->render();

		physics::GameObject* quit = LevelAssetManager::getGameObject("menuQuit");
		if (quit->isHit(math::Vector2(x, y)))
		{
			quit->_sprite.setColor(0xff8a8a8a);
			if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				window->close();
			}
		}
		else
			quit->_sprite.setColor(0xffffffff);
	};

public:
	~Game() {
		delete menu;
	}
};

int main()
{
	Game game;
	game.start();
	LevelAssetManager::clean();
	graphics::TextureManager::clean();
	return 0;
}
#endif