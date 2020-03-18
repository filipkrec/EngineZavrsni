
#if 0
#include <windows.system.h>
#include <thread>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include "Source/Graphics/Group.h"
#include "Source/Managers/AudioManager.h"
#include "Source/Managers/TextureManager.h"
#include "Source/Managers/LevelAssetManager.h"
#include "Source/Graphics/Camera.h"
#include "Source/Physics/Objects/Player.h"
#include "Game/Pickups/Ammo.h"
#include "Source/Physics/Objects/Inventory/WeaponObject.h"
#include <iostream>
#define FRICTION 1.0f
#define PROCESSING_INTERVAL 60

int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace objects;
	using namespace math;
	using namespace audio;
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer();
	layer->add(new Sprite(0.0f, 0.0f, 32.0f, 18.0f, 0xffff0f0f, 0));
	TextureManager::add(new Texture("Assets/test2.png"), "Planet");
	TextureManager::add(new Texture("Assets/test3.png"), "Space");
	TextureManager::add(new Texture("Assets/test.png"), "!!");
	TextureManager::add(new Texture("Assets/playertest.png"), "Player");
	TextureManager::add(new Texture("Assets/ammo.png"), "Ammo");
	TextureManager::add(new Texture("Assets/rifle.png"), "Rifle");
	TextureManager::add(new Texture("Assets/riflePickup.png"), "RiflePickup");

	TextureManager::add(new Texture("Assets/Character/Main_idle.png"), "Main_idle");
	TextureManager::add(new Texture("Assets/Character/Main_walking1.png"), "Main_walking1");
	TextureManager::add(new Texture("Assets/Character/Main_walking2.png"), "Main_walking2");

	TextureManager::add(new Texture("Assets/Character/Enemy_idle.png"), "Enemy_idle");
	TextureManager::add(new Texture("Assets/Character/Enemy_walking1.png"), "Enemy_walking1");
	TextureManager::add(new Texture("Assets/Character/Enemy_walking2.png"), "Enemy_walking2");

	Font* font = new Font("Assets/arial.ttf");
	font->setScale(16, 9);

	lam::LevelAssetManager::init(layer);
	lam::LevelAssetManager::setPlayer(new Player(GameObject(Sprite(0.0f, 0.0f, 2.0f, 2.0f, TextureManager::get("Main_idle"), 2), 100), 100, 150));
	lam::LevelAssetManager::add(new NPC(GameObject(Sprite(-8.0f, -8.0f, 1.0f, 1.0f, TextureManager::get("Enemy_idle"), 2),100),50,50),"NPC");
	lam::LevelAssetManager::add(new Label("100/100", -10.0f, 6.0f, 0xff00ff00, 0.5f, font, 16), "AmmoLabel");
	Weapon* weapon = new Weapon(Sprite(0.0f, 0.0f, 2.0f, 2.0f, TextureManager::get("Rifle"), 999), 0, 10, 10, 200.0f, 20.0f, 1, 100, 50, math::Vector2(1.0f, 0.0f));
	lam::LevelAssetManager::getPlayer()->setWeaponOffset(math::Vector2(0.5f, -0.4f));
	lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 32.0f, 18.0f, 0xffffffff,0), "Space");
	//lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), "Player");
	//lam::LevelAssetManager::add(new GameObject(Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), 100), "Player");
	lam::LevelAssetManager::add(new GameObject(Sprite(9.0f, 5.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 300), "Planet");
	lam::LevelAssetManager::add(new GameObject(Sprite(3.0f, -3.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 300), "Planet2");

	lam::LevelAssetManager::getPlayer()->setSight(45.0f,10.0f);
	lam::LevelAssetManager::getPlayer()->setAllegiance(Allegiance::GOOD);

	lam::LevelAssetManager::getPlayer()->addTexture(TextureManager::get("Main_walking1"), ActorState::STATE_MOVING);
	lam::LevelAssetManager::getPlayer()->addTexture(TextureManager::get("Main_walking2"), ActorState::STATE_MOVING);
	lam::LevelAssetManager::getPlayer()->setAnimationTimerForState(0.3f, ActorState::STATE_MOVING);

	Sprite* sprite = new Sprite(8.0f, 8.0f, 1.0f, 1.0f, 0xff00ff00, 0);
	
	lam::LevelAssetManager::getNPC("NPC")->setMoveToPoint(math::Vector2(8.0f,8.0f));
	/*
	lam::LevelAssetManager::getNPC("NPC")->setSight(45.0f,10.0f);
	lam::LevelAssetManager::getNPC("NPC")->setAIState(AIState::AI_STATE_DEFENSIVE);
	lam::LevelAssetManager::getNPC("NPC")->setAllegiance(Allegiance::BAD);
	lam::LevelAssetManager::getNPC("NPC")->addEnemyAllegiance(Allegiance::GOOD);
	*/

	Timer* timer = new Timer();
	Timer* timerTick = new Timer();
	int fps = 0;

	math::Vector2 mousePosition;
	while (!display->closed())
	{
		display->clear();
		lam::LevelAssetManager::processBegin(*display);
		fps++;
		display->getMousePosition(mousePosition);
		if (timerTick->elapsed() >= 1.0f / 60.0f)
		{
			Player* player = lam::LevelAssetManager::getPlayer();
			Hitbox* hitbox = lam::LevelAssetManager::getPlayer();
			GameObject* planet = lam::LevelAssetManager::getGameObject("Planet");
			Label* label = lam::LevelAssetManager::getLabel("AmmoLabel");
			if (player->getWeapon() != nullptr)
			{
				unsigned int maxclip = player->getWeapon()->getClipMax();
				unsigned int clip = player->getWeapon()->getClipCurrent();
				label->setText(std::to_string(clip) + " / " + std::to_string(maxclip));
			}
			/*
			if (planet->isHit(*hitbox))
				planet->getSprite()->swapTexture(TextureManager::get("!!"));
			else
				planet->getSprite()->swapTexture(TextureManager::get("Planet"));
			*/
			//Camera::getInstance()->move(Vector2(-0.01f, 0.0f));
			timerTick->reset();
		}


		if (timer->elapsed() >= 1.0f)
		{
			//std::cout << fps << std::endl;

			timer->reset();
			fps = 0;
		}

		lam::LevelAssetManager::processEnd(*display);
		layer->render();
		display->update();
	}
}
#endif

#if 0
struct test
{
	int x = 0;
	bool processed = false;
};

void funkcija(bool &finished, engine::Timer& x)
{
	while (true)
	{
		if(finished)
		std::cout << "threading" << std::endl;

		if (x.elapsed() >= 5.0f)
			finished = true;
	}
}

int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace objects;
	using namespace math;
	using namespace audio;
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer();

	Timer timer1;
	bool finished = false;
	std::thread t1(funkcija, std::ref(finished), std::ref(timer1));
	//,std::ref(x),std::ref(finished)

	while (true)
	{
		if (finished)
		{
			t1.join();
			break;
		}

		std::cout << "main" << std::endl;
	}



}
#endif

#if 0
#include "gorilla/ga.h"
#include "gorilla/gau.h"

#include <stdio.h>

static void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
{
	gc_int32* flag = (gc_int32*)(in_context);
	*flag = 1;
	ga_handle_destroy(in_handle);
}
int main(int argc, char** argv)
{
	gau_Manager* mgr;
	ga_Mixer* mixer;
	ga_Sound* sound;
	ga_Handle* handle;
	gau_SampleSourceLoop* loopSrc = 0;
	gau_SampleSourceLoop** pLoopSrc = &loopSrc;
	gc_int32 loop = 0;
	gc_int32 quit = 0;

	/* Initialize library + manager */
	gc_initialize(0);
	mgr = gau_manager_create();
	mixer = gau_manager_mixer(mgr);

	/* Create and play shared sound */
	if (!loop)
		pLoopSrc = 0;
	sound = gau_load_sound_file("Assets/Test.wav", "wav");
	handle = gau_create_handle_sound(mixer, sound, &setFlagAndDestroyOnFinish, &quit, pLoopSrc);
	ga_handle_play(handle);

	/* Bounded mix/queue/dispatch loop */
	while (!quit)
	{
		gau_manager_update(mgr);
		printf("%d / %d\n", ga_handle_tell(handle, GA_TELL_PARAM_CURRENT), ga_handle_tell(handle, GA_TELL_PARAM_TOTAL));
		gc_thread_sleep(1);
	}

	/* Clean up sound */
	ga_sound_release(sound);

	/* Clean up library + manager */
	gau_manager_destroy(mgr);
	gc_shutdown();

	return 0;
}
#endif