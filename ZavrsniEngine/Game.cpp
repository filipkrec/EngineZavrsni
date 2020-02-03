#include "Source/Engine.h"

class Game : public engine::Engine
{
	// na po�etku izvr�avanja !mora se implementirati!
	engine::Window* window = createWindow("Game", 800, 600, false);
	double x;
	double y;
	graphics::Layer* menu = new graphics::Layer(math::Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	graphics::Font* font = new graphics::Font("Assets/arial.ttf", 24);
	graphics::Label* fps = new graphics::Label("0", -15.0f, 8.0f, 0xff00ff00, font, 2);
	graphics::Sprite* background;
	graphics::Sprite* newGame;
	graphics::Sprite* score;
	graphics::Sprite* quit;
	graphics::Sprite* cursor;
	physics::Hitbox* hitbox;
	void init()
	{
		window->toggleCursor();
		font->setScale(800.0f / 32.0f, 600.0f / 18.0f);

		graphics::Texture* menuBackground = new graphics::Texture("Assets/city.png");
		graphics::Texture* menuNewgame = new graphics::Texture("Assets/Newgame.png");
		graphics::Texture* menuQuit = new graphics::Texture("Assets/Quit2.png");
		graphics::Texture* menuScore = new graphics::Texture("Assets/Score.png");
		graphics::Texture* menuCursor = new graphics::Texture("Assets/Cursor.png");

		background = new graphics::Sprite(-16.0f, -9.0f, 32.0f, 18.0f, menuBackground, 0);
		newGame = new graphics::Sprite(-4.0f, 0.0f, 8.0f, 8.0f, menuNewgame, 1);
		score = new graphics::Sprite(-4.0f, -3.0f, 8.0f, 8.0f, menuScore, 1);
		quit = new graphics::Sprite(-3.0f, -5.0f, 6.0f, 2.0f, menuQuit, 1);
		hitbox = new physics::Hitbox(quit, physics::SQUARE, 6.0f, 2.0f);
		cursor = new graphics::Sprite(0.0f, 0.0f, 2.0f, 2.0f, menuCursor, 100);
		menu->add(cursor);
		menu->add(background);
		menu->add(newGame);
		menu->add(score);
		menu->add(quit);
		menu->add(fps);
	}
	// jednom svake sekunde
	void tick() 
	{
		fps->setText(std::to_string(_FPS));
	}
	// 60 puta u sekundi
	void update() {}
	// Svaki loop !mora se implementirati!
	void render() 
	{

		window->getMousePosition(x, y);
		x = (x / 800) * 32 - 16;
		y = -((y / 600) * 18 - 9);
		cursor->setPosition(math::Vector2(x - 0.5f ,y-2.0f));
		menu->render();

		if (hitbox->isHit(math::Vector2(x, y)))
		{
			hitbox->getSprite()->setColor(0xff8a8a8a);
			if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				window->close();
			}
		}
		else
			hitbox->getSprite()->setColor(0xffffffff);
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
	return 0;
}