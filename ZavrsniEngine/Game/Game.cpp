#if 1
#include "../Source/Engine.h"
#include "Pickups/Ammo.h"
#include "Weapons/Rifle.h"
#include "Weapons/Laser.h"
#include "Spawners/Spawner.h"

class Game : public engine::Engine
{
	engine::Window* window = createWindow("Game", 800, 600, false);
	graphics::Layer* layer = new graphics::Layer();
	graphics::Font* font = new graphics::Font("Assets/arial.ttf");
	math::Vector2 mousePosition;
	std::vector<objects::Weapon*> weapons;
	std::vector<Spawner*> spawners;
	std::vector<math::Vector2> spawnerLocations;

	void (Game::*level)();
	bool skipRender = false;
	
	void init() //First level on init - MAIN MENU
	{
		window->toggleCursor();
		//window->toggleVsync();
		lam::LevelAssetManager::init(layer);
		audio::AudioManager::init();
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
		if (window->getKey(GLFW_KEY_P) && engine::Timer::isPaused())
			{
				window->clearInput();
				engine::Timer::unpause();
			}
		
		if (!engine::Timer::isPaused())
			lam::LevelAssetManager::processBegin(*window);

			(this->*level)();

			audio::AudioManager::update();

		if (!engine::Timer::isPaused())
			lam::LevelAssetManager::processEnd(*window);

		window->clearInput();
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
		Camera::getInstance()->recenter();

		//clearSpawners
		spawnerLocations.clear();

		//clearWeapons
		weapons.clear();

		//clearSpawners
		spawners.erase(
			std::remove_if(spawners.begin(), spawners.end(),
				[](Spawner* x) {
					delete x;
					return true;
				}),
			spawners.end());
		
		//clear layer
		layer->clear();

		//clear audio
		audio::AudioManager::clear();

		level = newLevel;
		(this->*newLevelInit)();
		skipRender = true;

		engine::Timer::unpause();
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

		audio::AudioManager::add(new audio::Audio("MenuMusic","Assets/Sounds/MenuMusic.wav"));
		audio::AudioManager::get("MenuMusic")->setLoopOnFinish(true);
		audio::AudioManager::get("MenuMusic")->play();
		//audio::AudioManager::add(new audio::Audio("MenuMusic","../Assets/Test.wav"));
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
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Main_dead.png"), "Main_dead");

		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_idle.png"), "Enemy_idle");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_walking1.png"), "Enemy_walking1");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_walking2.png"), "Enemy_walking2");
		graphics::TextureManager::add(new graphics::Texture("Assets/Character/Enemy_dead.png"), "Enemy_dead");

		graphics::TextureManager::add(new graphics::Texture("Assets/rifle.png"), "Rifle");

		graphics::TextureManager::add(new graphics::Texture("Assets/HPBar.png"), "HPBar");
		graphics::TextureManager::add(new graphics::Texture("Assets/UIAmmo.png"), "UIAmmo");
		graphics::TextureManager::add(new graphics::Texture("Assets/iconReload.png"), "iconReload");

		//PLAYER SETUP
		lam::LevelAssetManager::setPlayer(new objects::Player(objects::GameObject(graphics::Sprite(0.0f, 0.0f, 2.0f, 2.0f, graphics::TextureManager::get("Main_idle"), 5), 100,
			objects::Shape::SQUARE,1.5f), 100, 150));
		lam::LevelAssetManager::getPlayer()->setAllegiance(objects::Allegiance::GOOD);
		lam::LevelAssetManager::getPlayer()->addTexture(graphics::TextureManager::get("Main_walking1"), objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->addTexture(graphics::TextureManager::get("Main_walking2"), objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->addTexture(graphics::TextureManager::get("Main_dead"), objects::ActorState::STATE_DEAD);
		lam::LevelAssetManager::getPlayer()->setAnimationTimerForState(0.3f, objects::ActorState::STATE_MOVING);
		lam::LevelAssetManager::getPlayer()->setSight(0.01f, 0.01f);
		lam::LevelAssetManager::getPlayer()->setWeaponOffset(math::Vector2(0.5f, -0.4f));

		//NPC SETUP
		objects::NPC prototype(objects::GameObject(graphics::Sprite(-8.0f, -8.0f, 1.0f, 1.0f, graphics::TextureManager::get("Enemy_idle"), 3), 100), 50, 50);
		prototype.setAllegiance(objects::Allegiance::BAD);
		prototype.addEnemyAllegiance(objects::Allegiance::GOOD);
		prototype.setAIState(objects::AIState::AI_STATE_DEFENSIVE);
		prototype.setSight(60.0f, 7.0f);
		prototype.addTexture(graphics::TextureManager::get("Enemy_walking1"), objects::ActorState::STATE_MOVING);
		prototype.addTexture(graphics::TextureManager::get("Enemy_walking2"), objects::ActorState::STATE_MOVING);
		prototype.addTexture(graphics::TextureManager::get("Enemy_dead"), objects::ActorState::STATE_DEAD);

		spawnerLocations.push_back(math::Vector2(-31, -40)); //dolje ljevo
		spawnerLocations.push_back(math::Vector2(31, -41)); //dolje desno
		spawnerLocations.push_back(math::Vector2(31, 32)); //gore desno
		spawnerLocations.push_back(math::Vector2(-41, 32)); //gore ljevo

		Spawner* spawner = new Spawner(prototype,spawnerLocations.at(0), lam::LevelAssetManager::getPlayer()->getPosition(), 1.0f,1);
		spawners.push_back(spawner);

		weapons.push_back(new Rifle(math::Vector2(1.0f, 0.5f), math::Vector2(-0.5f, 0.0f), math::Vector2(1.0f, 0.0f), graphics::TextureManager::get("Rifle")));	
		weapons.push_back(new Laser());
		lam::LevelAssetManager::getPlayer()->setWeapon(weapons.at(0));

		//UI

		lam::LevelAssetManager::add(new graphics::Label("0", 0.0f, 0.0f, 0xff00ff00, 0.3f, font, 102), "FPS");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("FPS"), "FPS", math::Vector2(-14.0f, 8.0f));

		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 4.5f, 1.5f, graphics::TextureManager::get("HPBar"), 101), "HPBar");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getSprite("HPBar"), "HPBar", math::Vector2(10.0f, 7.0f));

		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 3.50f, 0.745f, 0xff0000ff,100),"Health");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getSprite("Health"), "Health", math::Vector2(8.4f, 6.6975f));

		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 1.5f, 0.75f, graphics::TextureManager::get("UIAmmo"), 101),"UIAmmo");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getSprite("UIAmmo"), "UIAmmo", math::Vector2(10.8f, -7.0f));

		lam::LevelAssetManager::add(new graphics::Label("0 / 0", 0.0f, 0.0f, 0xff00a5ff, 0.35f, font, 102), "AmmoClip");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("AmmoClip"), "AmmoClip", math::Vector2(8.0f, -7.05f));
		lam::LevelAssetManager::add(new graphics::Label("0", 0.0f, 0.0f, 0xff00a5ff, 0.35f, font, 102), "AmmoMax");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("AmmoMax"), "AmmoMax", math::Vector2(8.0f, -7.45f));

		lam::LevelAssetManager::add(new graphics::Sprite(0.0f, 0.0f, 2.0f, 1.2f, graphics::TextureManager::get("iconReload"), 101), "iconReload");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getSprite("iconReload"), "iconReload", math::Vector2(13.0f, -7.0f));

		lam::LevelAssetManager::add(new graphics::Label("PAUSED", 0.0f, 0.0f, 0x00696969, 2.0f, font, 102), "PAUSED");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("PAUSED"), "PAUSED", math::Vector2(-8.0f, 0.0f));

		lam::LevelAssetManager::add(new graphics::Label("PRESS Q TO QU1T", 0.0f, 0.0f, 0x00696969, 0.5f, font, 102), "QUIT");
		lam::LevelAssetManager::addUI(lam::LevelAssetManager::getLabel("QUIT"), "QUIT", math::Vector2(-8.0f, -4.0f));

		//SOUNDS


		audio::AudioManager::add(new audio::Audio("DemoMusic", "Assets/Sounds/DemoMusic.wav"));
		audio::AudioManager::add(new audio::Audio("Laser", "Assets/Sounds/EnemyLaser.wav"));
		audio::AudioManager::add(new audio::Audio("RifleReload", "Assets/Sounds/SniperReload.wav"));
		audio::AudioManager::add(new audio::Audio("RifleShoot", "Assets/Sounds/SniperShot.wav"));
		audio::AudioManager::get("DemoMusic")->play();
		audio::AudioManager::get("DemoMusic")->setLoopOnFinish(true);
		audio::AudioManager::get("DemoMusic")->setGain(0.0);
	}

	void LevelDemo()
	{
		//CURSOR
		window->getMousePosition(mousePosition);
		graphics::Sprite* cursor = lam::LevelAssetManager::getSprite("crosshairCursor");
		cursor->setPosition(mousePosition);

		if (!engine::Timer::isPaused())
		{
			//SPAWNERI
			for (Spawner* spawner : spawners)
			{
				math::Vector2& spawnerLoc = spawnerLocations.at(rand() % 4);
				if (!lam::LevelAssetManager::checkForNpcs(spawnerLoc))
				{
					spawner->setSpawnLocation(spawnerLoc);
					spawner->setDestination(lam::LevelAssetManager::getPlayer()->getPosition());


					objects::NPC* npc = spawner->Spawn();
					if (npc != nullptr)
					{
						lam::LevelAssetManager::add(npc, spawner->getSpawnName());
						objects::Weapon* weapon = (weapons.at(1));
						weapon = weapon->clone();
						weapons.push_back(weapon);
						npc->setWeapon(weapon);
					}
				}
			}

			//UI 

			objects::Player* pl = lam::LevelAssetManager::getPlayer();
			double tempHp = pl->getHealth();
			double maxHp = pl->getMaxHealth();
			double healthPercentage = tempHp / maxHp;
			if (healthPercentage < 100)
			{
				math::Vector2 originalHPPosition(10.15f, 7.07f);
				if (healthPercentage > 0)
				{
					lam::LevelAssetManager::getSprite("Health")->setWidth(3.50f * healthPercentage);
				}
			}
			lam::LevelAssetManager::getSprite("Health")->setOffset(math::Vector2(lam::LevelAssetManager::getSprite("Health")->getSize().x / 2, lam::LevelAssetManager::getSprite("Health")->getSize().y / 2));

			if (lam::LevelAssetManager::getPlayer()->getWeapon() != nullptr)
			{
				lam::LevelAssetManager::getLabel("AmmoClip")
					->setText(std::to_string(lam::LevelAssetManager::getPlayer()->getWeapon()->getClipCurrent()) + "/" + std::to_string(lam::LevelAssetManager::getPlayer()->getWeapon()->getClipMax()));
				lam::LevelAssetManager::getLabel("AmmoMax")
					->setText(std::to_string(lam::LevelAssetManager::getPlayer()->getWeapon()->getAmmoCurrent()));
				lam::LevelAssetManager::getSprite("iconReload")->setColor(lam::LevelAssetManager::getPlayer()->getWeapon()->isReloading() ? 0xffffffff : 0x00ffffff);
			}

			lam::LevelAssetManager::getLabel("PAUSED")->setColor(0x00696969);

			//PLAYER
			if (lam::LevelAssetManager::getPlayer()->isDead())
			{
				lam::LevelAssetManager::getPlayer()->setZindex(2);
				lam::LevelAssetManager::getPlayer()->setScale(math::Vector2(1.4f, 0.8f));
				lam::LevelAssetManager::getLabel("QUIT")->setColor(0xff696969);
				if (window->getKeyPressed(GLFW_KEY_Q))
				{
					swapLevel(&Game::levelMenuInit, &Game::levelMenu);
				}
			}
			else
			{
				lam::LevelAssetManager::getLabel("QUIT")->setColor(0x00696969);

				if (window->getKey(GLFW_KEY_P))
				{
					engine::Timer::pause();
				}
			}

			//NPCS
			objects::NPC* npc;
			int i = 0;
			while (true)
			{
				npc = lam::LevelAssetManager::getNPC(i);
				if (npc == nullptr)
					break;
				if (npc->isDead())
				{
					npc->setScale(math::Vector2(1.6f, 1.0f));
				}
				++i;
			}
		}
		else
		{
			lam::LevelAssetManager::getLabel("PAUSED")->setColor(0xff696969);
			lam::LevelAssetManager::getLabel("QUIT")->setColor(0xff696969);

			if (window->getKeyPressed(GLFW_KEY_Q))
			{
				swapLevel(&Game::levelMenuInit,&Game::levelMenu);
			}
		}
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