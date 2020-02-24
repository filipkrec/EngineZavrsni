#include "LevelAssetManager.h"
#include <algorithm>

namespace lam {
	engine::Timer* LevelAssetManager::_timer = new engine::Timer();

	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_sprites;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_gameObjects;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_NPCs;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_labels;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_pickups;
	std::vector<graphics::Line*> LevelAssetManager::_shots;
	objects::Player* LevelAssetManager::_player = nullptr;

	void LevelAssetManager::process(engine::Window& window)
	{
		if (_timer->elapsed() >= 1.0f / PROCESSING_INTERVAL)
		{
			//fill secondary objects for processing
			objects::GameObject* playerObject = (objects::GameObject*)_player;
			std::vector<activeObject> allObjects;
			allObjects.push_back(activeObject((void*)playerObject, "Player"));
			allObjects.insert(allObjects.end(), _gameObjects.begin(), _gameObjects.end());
			allObjects.insert(allObjects.end(), _NPCs.begin(), _NPCs.end());

			objects::GameObject* gameObject1;
			objects::GameObject* gameObject2;

			for (activeObject NPC : _NPCs)
			{
				objects::NPC* npc = (objects::NPC*)NPC._object;
				std::vector<activeObject> sightedObjects;
				for (activeObject gameObject : allObjects)
				{
					gameObject1 = (objects::GameObject*)gameObject._object;
					if (npc != NPC._object)
					{
						if (npc->objectIsInSight(*gameObject1))
							sightedObjects.push_back(gameObject);
					}
				}
			}

			if (_player != nullptr)
			{
				for (activeObject pickup : _pickups)
				{
					objects::Pickup* temp = (objects::Pickup*)pickup._object;
					if (temp->isHit(*(objects::Hitbox*)_player))
					{
						_player->addPickupable(temp);
					}
				}
				_player->process(window);
				_player->clearPickupable();
			}
			
			for (activeObject npc: _NPCs)
			{
				((objects::NPC*)npc._object)->process();
			}
			
			//colission

			for (activeObject gameObject : allObjects)
			{
				objects::GameObject* gameObject1 = (objects::GameObject*)gameObject._object;
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
						if (gameObject2->willBeHit(*hitbox1, gameObject2->_nextMove))
						{
							gameObject1->collide(*gameObject2);
						}
						else if (gameObject2->isHit(*hitbox1))
						{
							gameObject1->collide(*gameObject2);
						}
					}
				}
			}
		
			//shot detection
			if (_player->getWeapon() != nullptr)
			{
				for (math::Vector2 firedShot : _player->getWeapon()->_firedShots)
				{
					std::vector<std::pair<objects::GameObject*, math::Vector2>> shotObjects;
					for (activeObject gameObject : _gameObjects)
					{
						objects::GameObject* gameObject1 = (objects::GameObject*)gameObject._object;
						if (gameObject1 != _player && gameObject1->getLineIntersection(_player->getWeapon()->getShotPosition(), firedShot) != math::Vector4(0, 0, 0, 0))
						{
							math::Vector4 clipPoints = gameObject1->getLineIntersection(_player->getWeapon()->getShotPosition(), firedShot);
							shotObjects.push_back(std::make_pair(gameObject1, math::Vector2(clipPoints.x, clipPoints.y)));
							shotObjects.push_back(std::make_pair(gameObject1, math::Vector2(clipPoints.z, clipPoints.w)));
						}
					}
					if (!shotObjects.empty())
					{
						std::pair<objects::GameObject*, math::Vector2> closestShot = *std::min_element(shotObjects.begin(), shotObjects.end(),
							[&](const std::pair<objects::GameObject*, math::Vector2>& s1, const std::pair<objects::GameObject*, math::Vector2>& s2)
							{return s1.second.distanceFrom(firedShot) > s2.second.distanceFrom(firedShot); });
						math::Vector2 hitPoint = closestShot.second;
						graphics::Line* lineSprite = new graphics::Line(math::Vector2(_player->getWeapon()->getShotPosition()), math::Vector2(hitPoint));
						_shots.push_back(lineSprite);
						math::Vector2 unitVector = hitPoint - _player->getWeapon()->getShotPosition();
						unitVector = unitVector.calculateUnitVector(unitVector.x, unitVector.y);
						closestShot.first->calculateColission(math::Vector3(unitVector.x, unitVector.y, _player->getWeapon()->getForce()));
						shotObjects.clear();
					}
					else
					{
						graphics::Line* lineSprite = new graphics::Line(math::Vector2(_player->getWeapon()->getShotPosition()), firedShot);
						_shots.push_back(lineSprite);
					}
				}
			}

			//movement
			for (activeObject gameObject : allObjects)
			{
				gameObject1 = (objects::GameObject*)gameObject._object;
				gameObject1->move();
			}

			//cleanup
			
			for (activeObject pickup : _pickups)
			{
				objects::Pickup* temp = (objects::Pickup*)pickup._object;
				temp->processTime();
			}

			for (graphics::Line* line : _shots)
			{
				line->tick();
			}

			_shots.erase(
				std::remove_if(_shots.begin(), _shots.end(),
					[](graphics::Line* x) {
						return x->getDuration() == 0;
					}),
				_shots.end());

			_pickups.erase(
				std::remove_if(_pickups.begin(), _pickups.end(),
					[](activeObject x) {
						objects::Pickup* temp = (objects::Pickup*)x._object;
						bool destroy = temp->toDestroy();
						if (destroy)
						{
							delete temp;
						}
						return destroy;
					}),
				_pickups.end());
			
			window.clearInput();
			_timer->reset();
		}
	}

	void LevelAssetManager::init(objects::Player* player)
	{
		if (player == nullptr)
			_player = new objects::Player();
		else
		{
			_player = player;
			_player->getSprite()->DoNotDestroySprite();
		}
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

		_labels.clear();
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

		_gameObjects.clear();
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

		_NPCs.clear();
	}

	objects::NPC* LevelAssetManager::getNPC(const std::string& name)
	{
		for (activeObject npc : _NPCs)
		{
			if (npc._name == name)
				return (objects::NPC*)npc._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(objects::Pickup* pickup, const std::string& name)
	{
		_pickups.push_back(activeObject((void*)pickup, name));
	}

	void LevelAssetManager::cleanPickups()
	{
		for (activeObject pickup : _pickups)
			delete (objects::Pickup*)pickup._object;

		_pickups.clear();
	}

	objects::Pickup* LevelAssetManager::getPickup(const std::string& name)
	{
		for (activeObject pickup : _pickups)
		{
			if (pickup._name == name)
				return (objects::Pickup*)pickup._object;
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
			layer->add(((objects::GameObject*)gameObject._object)->getSprite());
		}

		for (activeObject npc : _NPCs)
		{
			layer->add((((objects::NPC*)npc._object)->getSprite()));
		}

		for (activeObject label : _labels)
		{
			layer->add((graphics::Label*)label._object);
		}

		for (activeObject pickup : _pickups)
		{
			layer->add(((objects::Pickup*)pickup._object)->getSprite());
		}

		if (_player != nullptr)
			layer->add((_player->getSprite()));
	}

	void LevelAssetManager::refreshShots(graphics::Layer* layer)
	{
		for (graphics::Line* shot : _shots)
		{
			if(shot->isNew())
			layer->add((graphics::Sprite*)shot);
		}
	}


	void LevelAssetManager::clean()
	{
		cleanGameObjects();
		cleanNPCs();
		cleanSprites();
		cleanLabels();
	}
}