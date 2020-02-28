#include "LevelAssetManager.h"
#include <algorithm>
#define STEPFIDELITY 2
#define STEPDISTANCE 1.0f


namespace lam {
	graphics::Layer* LevelAssetManager::_layer;
	engine::Timer* LevelAssetManager::_timer = new engine::Timer();

	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_sprites;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_gameObjects;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_NPCs;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_labels;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_pickups;
	std::vector<graphics::Line*> LevelAssetManager::_shots;
	objects::Player* LevelAssetManager::_player = nullptr;

	void LevelAssetManager::processBegin(engine::Window& window)
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
				for (activeObject gameObject : allObjects)
				{
					gameObject1 = (objects::GameObject*)gameObject._object;
					if (npc != NPC._object)
					{
						if (npc->objectIsInSight(*gameObject1))
							npc->addSighted(gameObject1);
					}
				}

				if (!npc->isPatroling() && npc->isPatrol())
				{
					npc->togglePatroling();
					npc->setMoveToPoint(npc->getPatrolEndPoint());
					npc->setPatrolOriginPoint(npc->getSpritePosition());
				}
				if (npc->isPatroling() && !npc->isPatrol())
				{
					npc->togglePatroling();
				}

				if (npc->getSpritePosition().distanceFrom(npc->getMoveToPoint()) <= STEPDISTANCE)
				{
					if (!npc->isPointReached())
						npc->togglePointReached();
				}

				//pathfinding
				if (!npc->isPointReached())
				{
					math::Vector2 checkPoint = npc->getMoveToCheckPoint();
					if (npc->getSpritePosition().distanceFrom(checkPoint) <= STEPDISTANCE)
					{
						if(!npc->isCheckpointReached())
						npc->toggleCheckpointReached();
					}

					//calculate path vrijednosti : X = X smjera, Y = Y smjera, Z = udaljenost od odredišta, W = broj koraka do odredišta
					if (npc->isCheckpointReached() || npc->isColided())
					{
						if (npc->isCheckpointReached())
						npc->toggleCheckpointReached();

						std::vector<math::Vector3> passed;
						math::Vector2 checkPoint;
						bool checkPointChecked = false; 
						const math::Vector4 nextStep = calculatePath(passed, checkPoint, checkPointChecked, npc->getSpritePosition(), npc->getMoveToPoint(), 0, 0, *npc);
						if(nextStep.z < STEPDISTANCE)
						if (nextStep.x == 0.0f && nextStep.y == 0.0f && nextStep.w == 0.0f)
						{
							npc->setMoveDirection(math::Vector2::calculateUnitVector(npc->getSpritePosition() - npc->getMoveToPoint()));
						}
						else
						{
							npc->setMoveDirection(math::Vector2(nextStep.x, nextStep.y));
							if (checkPointChecked)
								npc->setMoveToCheckPoint(checkPoint);
							else
								npc->setMoveToCheckPoint(npc->getMoveToPoint());
						}
						passed.clear();
					}
					npc->moveInDirection();
				}
				else
				{
					npc->setMoveDirection(math::Vector2(0.0f, 0.0f));
					if (npc->isPatroling())
					{
						npc->setMoveToPoint(npc->getMoveToPoint() == npc->getPatrolEndPoint() ? npc->getPatrolOriginPoint() : npc->getPatrolEndPoint());
					}
				}
			}


			if (_player != nullptr)
			{
					for (activeObject gameObject : allObjects)
					{
						gameObject1 = (objects::GameObject*)gameObject._object;
							if (_player != gameObject1)
							{
								if (_player->objectIsInSight(*gameObject1))
									_player->addSighted(gameObject1);
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
				((objects::NPC*)npc._object)->process(window);
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
				bool collided = false;
				for (activeObject gameObjectOther : allObjects)
				{
					gameObject2 = (objects::GameObject*)gameObjectOther._object;
					objects::Hitbox* hitbox1 = (objects::Hitbox*)gameObject1;
					if (gameObject1 != gameObject2)
					{
						if (gameObject2->willBeHit(*hitbox1, gameObject2->_nextMove))
						{
							gameObject1->collide(*gameObject2);
							collided = true;
						}
						else if (gameObject2->isHit(*hitbox1))
						{
							gameObject1->collide(*gameObject2);
							collided = true;
						}
					}
				}
				if (!collided && gameObject1->isColided())
				{
					gameObject1->toggleColided();
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
						unitVector = math::Vector2::calculateUnitVector(unitVector);
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
		}
	}

	void LevelAssetManager::processEnd(engine::Window& window)
	{
		if (_timer->elapsed() >= 1.0f / PROCESSING_INTERVAL)
		{
			for (graphics::Line* shot : _shots)
			{
				if (shot->isNew())
					_layer->add((graphics::Sprite*)shot);
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

			for (activeObject npc : _NPCs)
			{
				objects::NPC* temp = (objects::NPC*)npc._object;
				if (temp->getActorTimer() >= 1.0f)
					temp->resetActorTimer();
			}

			if (_player->getActorTimer() >= 1.0f)
				_player->resetActorTimer();

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

	void LevelAssetManager::init(objects::Player* player, graphics::Layer* layer)
	{
			_player = player;
			_layer = layer;
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
			{
				objects::NPC* npcreturn = (objects::NPC*)npc._object;
				return npcreturn;
			}
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
			((objects::GameObject*)gameObject._object)->getSprite()->DoNotDestroySprite();
			layer->add(((objects::GameObject*)gameObject._object)->getSprite());
		}

		for (activeObject npc : _NPCs)
		{
			objects::NPC* temp = (objects::NPC*)npc._object;
			temp->init();
			layer->add(((temp->getSprite())));
		}

		for (activeObject label : _labels)
		{
			layer->add((graphics::Label*)label._object);
		}

		for (activeObject pickup : _pickups)
		{
			((objects::Pickup*)pickup._object)->getSprite()->DoNotDestroySprite();
			layer->add(((objects::Pickup*)pickup._object)->getSprite());
		}

		if (_player != nullptr)
			_player->init();
			layer->add((_player->getSprite()));
	}


	void LevelAssetManager::clean()
	{
		cleanGameObjects();
		cleanNPCs();
		cleanSprites();
		cleanLabels();
	}


	math::Vector2 LevelAssetManager::calculatePath(const math::Vector2& goal,const objects::NPC& npc)
	{
		std::vector<math::Vector3> endpoints;
		std::vector<math::Vector3> path;
		std::vector<math::Vector3> passed;
		std::vector<math::Vector2>& directionsAll = objects::NPC::directionsAll;

		if (goal.distanceFrom(npc.getSpritePosition()) < STEPDISTANCE)
			return math::Vector2(0, 0);

		endpoints.push_back(math::Vector3(npc.getSpritePosition().x, npc.getSpritePosition().y, 0));
		bool pathBlocked = false;
		// vector3 - X,Y,STEPS
		//pathBlocked(nextPositionTemp, npc)
		while (true)
		{
			if (endpoints.empty())
				return math::Vector2(0, 0);
			std::vector<math::Vector2> directions;
			//get endpoint least distance from end with least steps
			std::vector<math::Vector3>::iterator it = min_element(endpoints.begin(), endpoints.end(),
				[&](const math::Vector3& x, const math::Vector3& y)
				{
					return math::Vector2(x.x, x.y).distanceFrom(goal) < math::Vector2(y.x, y.y).distanceFrom(goal)
						&& x.z < y.z;
				});
			math::Vector3& endpointCurrent = *it; //get endpoint

			math::Vector2 currentPosition = math::Vector2(endpointCurrent.x, endpointCurrent.y);

			//if endpoint near end return first step;
			float smallestDistance = currentPosition.distanceFrom(goal);
			if (smallestDistance < STEPDISTANCE)
			{
					math::Vector2 to(path.at(1).x, path.at(1).y);
					return math::Vector2::calculateUnitVector(to - npc.getSpritePosition);
			}

			//if shorter path_from exists, lower endpoint steps
			for (math::Vector2 direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				for (math::Vector3& pathPoint : path)
					if (pathPoint.x == nextPositionTemp.x && pathPoint.y == nextPositionTemp.y && pathPoint.z + 1 < endpointCurrent.z)
						endpointCurrent.z = pathPoint.z + 1;
			}

			//only directions to unpassed and unblocked points
			for (math::Vector2 direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				if (!pathBlocked && !std::any_of(passed.begin(), passed.end(), [&](const math::Vector3& x)
					{return x.x == nextPositionTemp.x && x.y == nextPositionTemp.y; }))
					directions.push_back(direction);
			}

			//get smallest distance from available directions
			for (math::Vector2 direction : directions)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				float distance = nextPositionTemp.distanceFrom(goal);
				if (distance < smallestDistance)
				{
					smallestDistance = distance;
				}
			}

			//move endpoint to next, if more than 1 create additional
			bool multipleEndpoints = false;

			for (math::Vector2 direction : directions)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				float distance = nextPositionTemp.distanceFrom(goal);
				if (distance == smallestDistance)
				{
					if (!multipleEndpoints)
					{
						endpointCurrent.x = nextPositionTemp.x;
						endpointCurrent.y = nextPositionTemp.y;
						endpointCurrent.z++;
					}
					else
					{
						endpoints.push_back(math::Vector3(nextPositionTemp.x, nextPositionTemp.y, endpointCurrent.z));
					}
				}
			}
		}
	}

	bool LevelAssetManager::pathBlocked(const math::Vector2& unitVector, const objects::NPC& npc)
	{
		return false;
		for (activeObject gameObject : _gameObjects)
		{
			const objects::GameObject& gameObject1 = *(objects::GameObject*) gameObject._object;
			if (gameObject1.willBeHit((objects::Hitbox)npc, unitVector))
				return true;
		}
		return false;
	}
}