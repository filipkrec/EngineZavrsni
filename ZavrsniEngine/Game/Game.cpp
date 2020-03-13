#if 1
#include "../Source/Engine.h"
#include "Pickups/Ammo.h"
#include "Weapons/Rifle.h"
#include "Spawners/Spawner.h"

class Game : public engine::Engine
{
	engine::Window* window = createWindow("Game", 800, 600, false);
	graphics::Layer* layer = new graphics::Layer();
	graphics::Font* font = new graphics::Font("Assets/arial.ttf");
	math::Vector2 mousePosition;
	std::vector<objects::Weapon*> weapons;
	std::vector<Spawner*> spawners;

	void (Game::*level)();
	bool skipRender = false;
	
	void init() //First level on init - MAIN MENU
	{
		window->toggleCursor();
		//window->toggleVsync();
		lam::LevelAssetManager::init(layer);
		swapLevel(&Game::levelMenuInit, &Game::levelMenu);
	}
	// jednom svake sekunde
	void tick() 
	{
		std::cout << mousePosition.x << "," << mousePosition.y << std::endl;
		lam::LevelAssetManager::getLabel("FPS")->setText(std::to_string(_FPS));
	}
	// 60 puta u sekundi
	void update() {
		lam::LevelAssetManager::processBegin(*window);

		(this->*level)();

		lam::LevelAssetManager::processEnd(*window);
	}
	// Svaki loop !mora se implementirati!
	void render() 
	{

		layer->render();
	};

	void swapLevel(void (Game::* newLevelInit)(),void (Game::* newLevel)())
	{
		lam::LevelAssetManager::clean();
		graphics::TextureManager::clean();
		layer->clear();
		level = newLevel;
		(this->*newLevelInit)();
		skipRender = true;
	}

	//MENU CODE
	void levelMenuInit()
	{
		graphics::TextureManager::add(new graphics::Texture("Assets/city.png"), "menuBackground");
		graphics::TextureManager::add(new graphics::Texture("Assets/Newgame.png"), "menuNewgame");
		graphics::TextureManager::add(new graphics::Texture("Assets/Quit2.png"), "menuQuit");
		graphics::TextureManager::add(new graphics::Texture("Assets/Score.png"), "menuScore");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor4.png"), "menuCursor");
		graphics::TextureManager::add(new graphics::Texture("Assets/Cursor5.png"), "menuCursor2");
		lam::LevelAssetManager::add(new graphics::Label("0", -14.0f, 8.0f, 0xff00ff00, 0.3f, font, 4), "FPS");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 32.0f, 18.0f, graphics::TextureManager::get("menuBackground"), 0), "background");
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(0.0f, 3.0f, 8.0f, 2.0f, graphics::TextureManager::get("menuNewgame"), 1), 0), "menuNewGame");
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(0.0f, 0.0f, 8.0f, 2.0f, graphics::TextureManager::get("menuScore"), 2), 0), "menuScore");
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(0.0f, -3.0f, 6.0f, 2.0f, graphics::TextureManager::get("menuQuit"), 3), 0), "menuQuit");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 1.5f, 1.5f, graphics::TextureManager::get("menuCursor"), 100), "menuCursor");
	}

	void levelMenu()
	{
		window->getMousePosition(mousePosition);
		graphics::Sprite* cursor = lam::LevelAssetManager::getSprite("menuCursor");

		cursor->setPosition(mousePosition);
		if (window->getMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			cursor->swapTexture(graphics::TextureManager::get("menuCursor2"));
		else
			cursor->swapTexture(graphics::TextureManager::get("menuCursor"));

		objects::GameObject* newGame = lam::LevelAssetManager::getGameObject("menuNewGame");
		objects::GameObject* score = lam::LevelAssetManager::getGameObject("menuScore");
		objects::GameObject* quit = lam::LevelAssetManager::getGameObject("menuQuit");

		mousePosition.x -= cursor->getSize().x / 2;
		mousePosition.y += cursor->getSize().y / 2;
		if (newGame->isHit(mousePosition))
		{
			newGame->setColor(0xff8a8a8a);
			if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				swapLevel(&Game::levelDemoInit, &Game::LevelDemo);
			}
		}
		else if (score->isHit(mousePosition))
		{
			score->setColor(0xff8a8a8a);
			if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
			}
		}
		else if (quit->isHit(mousePosition))
		{
			quit->setColor(0xff8a8a8a);
			if (window->getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				window->close();
			}
		}
		else
		{
			newGame->setColor(0xffffffff);
			score->setColor(0xffffffff);
			quit->setColor(0xffffffff);
		}
	}

	//DEMO LEVEL CODE
	void levelDemoInit()
	{
		graphics::TextureManager::add(new graphics::Texture("Assets/crosshair.png"), "crosshairCursor");
		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 1.5f, 1.5f, graphics::TextureManager::get("crosshairCursor"), 100), "crosshairCursor");

		lam::LevelAssetManager::add(new graphics::Label("0", 0.0f, 0.0f, 0xff00ff00, 0.3f, font, 101), "FPS");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("FPS"), "FPS", math::Vector2(-14.0f, 8.0f));

		graphics::TextureManager::add(new graphics::Texture("Assets/Ground.png"), "floorTexture");
		graphics::TextureManager::add(new graphics::Texture("Assets/Wall.png"), "wallTexture");

		Camera::getInstance()->followPlayer();

		int width = 4;
		int height = 4;
		for (int x = -width; x < width; ++x)
		{
				for (int y = -height; y < height; ++y)
			{
				lam::LevelAssetManager::add(new graphics::Sprite(x*10, y*10, 10.0f, 10.0f, graphics::TextureManager::get("floorTexture"), 0), "FLOOR" +std::to_string(x) + "." + std::to_string(y));
			}
		}


		//WALLS BEGIN
		int x;
		int y;
		for (x = -width * 10 - 5; x <= width * 10 - 5; x += 2)
		{
			y = 10 * height;
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x, y - 5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(x) + "." + std::to_string(y));
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x, -y - 5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(x) + "." + std::to_string(-y));
		}

		for (y = -height * 10 - 5; y <= height * 10 - 5; y += 2)
		{
			x = 10 * width;
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x -5, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(x) + "." + std::to_string(y));
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-x - 5, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		}

		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(7, 7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(7, 5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(5, 7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));

		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-5, 5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-7, 5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-5, 7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));

		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-5, -5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-7, -5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-5, -7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));

		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(7, -7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(7, -5, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(5, -7, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));

		for (y = -10; y < 25; y+= 2)
		{
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-20, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		}

		for (x = -22; x < 0; x += 2)
		{
			lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-x, -12, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
		}

		for (x = 25; x < 35; x += 2)
		{
			for (y = -30; y < 30; y += 2)
			{
				if(rand()%10 == 0)
					lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
				
				if (rand() % 10 == 0)
					lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(-x, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
			}
		}

		for (y = 20; y < 30; y += 2)
		{
			for (x = -35; x < 35; x += 2)
			{
				if (rand() % 10 == 0)
					lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x, y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));

				if (rand() % 10 == 0)
					lam::LevelAssetManager::add(new objects::GameObject(graphics::Sprite(x, -y, 2.0f, 2.0f, graphics::TextureManager::get("wallTexture"), 1), 0, true), "WALL" + std::to_string(-x) + "." + std::to_string(y));
			}
		}
		//WALLS END

		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Main_idle.png"), "Main_idle");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Main_walking1.png"), "Main_walking1");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Main_walking2.png"), "Main_walking2");

		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_idle.png"), "Enemy_idle");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_walking1.png"), "Enemy_walking1");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_walking2.png"), "Enemy_walking2");

		graphics::TextureManager::add(new graphics::Texture("Assets/rifle.png"), "Rifle");

		//PLAYER SETUP
		lam::LevelAssetManager::setPlayer(new objects::Player(objects::GameObject(graphics::Sprite(0.0f, 0.0f, 2.0f, 2.0f, graphics::TextureManager::get("Main_idle"), 5), 100), 100, 150));
		lam::LevelAssetManager::getPlayer()->setAllegiance(objects::Allegiance::GOOD);
		lam::LevelAssetManager::getPlayer()->addTexture(graphics::TextureManager::get("Main_walking1"), objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->addTexture(graphics::TextureManager::get("Main_walking2"), objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->setAnimationTimerForState(0.3f, objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->setSight(0.01f, 0.01f);
		lam::LevelAssetManager::getPlayer()->setWeaponOffset(math::Vector2(0.5f, -0.4f));

		//NPC SETUP
		objects::NPC prototype(objects::GameObject(graphics::Sprite(-8.0f, -8.0f, 1.0f, 1.0f, graphics::TextureManager::get("Enemy_idle"), 2), 100), 50, 50);
		prototype.setSight(45.0f, 6.0f);

		math::Vector2 destination(rand() % 500 * 0.01, rand() % 500 * 0.01);
		math::Vector2 spawnerLocation(-31, -40);

		Spawner* spawner = new Spawner(prototype,spawnerLocation,destination,5.0f,1);
		spawners.push_back(spawner);

		weapons.push_back(new Rifle(math::Vector2(1.0f, 0.5f), math::Vector2(-0.5f, 0.0f), math::Vector2(1.0f, 0.0f), graphics::TextureManager::get("Rifle")));
		lam::LevelAssetManager::getPlayer()->setWeapon(weapons.at(0));
	}

	void LevelDemo()
	{
		window->getMousePosition(mousePosition);
		graphics::Sprite* cursor = lam::LevelAssetManager::getSprite("crosshairCursor");
		for (Spawner* spawner : spawners)
		{
			objects::NPC* npc = spawner->Spawn();
			if(npc != nullptr)
			lam::LevelAssetManager::add(npc,spawner->getSpawnName());
		}

		cursor->setPosition(mousePosition);
	}

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