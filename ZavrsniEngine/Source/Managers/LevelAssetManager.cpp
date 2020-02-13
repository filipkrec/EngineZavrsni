#include "LevelAssetManager.h"

namespace lam {
	engine::Timer* LevelAssetManager::_timer = new engine::Timer();

	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_sprites;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_gameObjects;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_NPCs;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_labels;
	objects::Player* LevelAssetManager::_player = nullptr;

	void LevelAssetManager::process(const engine::Window& window)
	{
		if (_timer->elapsed() >= 1.0f / PROCESSING_INTERVAL)
		{
			if (_player != nullptr)
				_player->process(window);

			objects::GameObject* playerObject = (objects::GameObject*)_player;
			for (activeObject npc: _NPCs)
			{
				((objects::NPC*)npc._object)->process();
			}
			
			//colission
			std::vector<activeObject> allObjects;
			allObjects.push_back(activeObject((void*)playerObject,"Player"));
			allObjects.insert(allObjects.end(), _gameObjects.begin(), _gameObjects.end());
			allObjects.insert(allObjects.end(), _NPCs.begin(), _NPCs.end());
			
			objects::GameObject* gameObject1;
			objects::GameObject* gameObject2;

			for (activeObject gameObject : allObjects)
			{
				gameObject1 = (objects::GameObject*)gameObject._object;
				gameObject1->savePreviousForce();
				gameObject1->calculateNextMove();
			}
			

			
			for (activeObject gameObject : allObjects)
			{
				gameObject1 = (objects::GameObject*)gameObject._object;
				for (activeObject gameObjectOther : allObjects)
				{
					gameObject2 = (objects::GameObject*)gameObjectOther._object;
					objects::Hitbox* hitbox1 = (objects::Hitbox*)gameObject1;
					if (gameObject1 != gameObject2)
					{
						if (gameObject2->willBeHit(*hitbox1,gameObject2->_nextMove))
							{
							gameObject1->collide(*gameObject2);
							}
					}
				}
			}

			//movement
			for (activeObject gameObject : allObjects)
			{
				gameObject1 = (objects::GameObject*)gameObject._object;
				gameObject1->move();
			}
			
			_timer->reset();
		}
	}

	void LevelAssetManager::init(objects::Player* player)
	{
		if (player == nullptr)
			_player = new objects::Player();
		else
			_player = player;
	}

	void LevelAssetManager::add(graphics::Sprite* sprite, const std::string& name)
	{
		_sprites.push_back(activeObject((void*)sprite, name));
	}

	void LevelAssetManager::cleanSprites()
	{
		for (activeObject sprite : _sprites)
			delete (graphics::Sprite*)sprite._object;

		_sprites.clear();
	}

	graphics::Sprite* LevelAssetManager::getSprite(const std::string& name)
	{
		for (activeObject sprite : _sprites)
		{
			if (sprite._name == name)
				return (graphics::Sprite*)sprite._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(graphics::Label* label, const std::string& name)
	{
		_labels.push_back(activeObject((void*)label, name));
	}

	void LevelAssetManager::cleanLabels()
	{
		for (activeObject label : _labels)
			delete (graphics::Label*)label._object;

		_sprites.clear();
	}

	graphics::Label* LevelAssetManager::getLabel(const std::string& name)
	{
		for (activeObject label : _labels)
		{
			if (label._name == name)
				return (graphics::Label*)label._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(objects::GameObject* gameObject, const std::string& name)
	{
		_gameObjects.push_back(activeObject((void*)gameObject, name));
	}

	void LevelAssetManager::cleanGameObjects()
	{
		for (activeObject gameObject : _gameObjects)
			delete (objects::GameObject*)gameObject._object;

		_sprites.clear();
	}

	objects::GameObject* LevelAssetManager::getGameObject(const std::string& name)
	{
		for (activeObject gameObject : _gameObjects)
		{
			if (gameObject._name == name)
				return (objects::GameObject*)gameObject._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(objects::NPC* NPC, const std::string& name)
	{
		_NPCs.push_back(activeObject((void*)NPC, name));
	}

	void LevelAssetManager::cleanNPCs()
	{
		for (activeObject NPC : _NPCs)
			delete (objects::NPC*)NPC._object;

		_sprites.clear();
	}

	objects::NPC* LevelAssetManager::getNPC(const std::string& name)
	{
		for (activeObject npc : _gameObjects)
		{
			if (npc._name == name)
				return (objects::NPC*)npc._object;
		}
		return nullptr;
	}

	objects::Player* LevelAssetManager::getPlayer()
	{
		return _player;
	}


	void LevelAssetManager::addToLayer(graphics::Layer* layer)
	{
		for (activeObject sprite : _sprites)
		{
			layer->add((graphics::Sprite*)sprite._object);
		}
		for (activeObject gameObject : _gameObjects)
		{
			graphics::Sprite* temp = (((objects::GameObject*)gameObject._object)->getSprite());
			layer->add(temp);
		}

		for (activeObject npc : _NPCs)
		{
			layer->add((((objects::NPC*)npc._object)->getSprite()));
		}

		for (activeObject label : _labels)
		{
			layer->add((graphics::Label*)label._object);
		}

		if (_player != nullptr)
			layer->add((_player->getSprite()));
	}


	void LevelAssetManager::clean()
	{
		cleanGameObjects();
		cleanNPCs();
		cleanSprites();
		cleanLabels();
	}
}