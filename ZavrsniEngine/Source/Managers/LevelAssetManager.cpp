#include "LevelAssetManager.h"
#include <algorithm>
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
					const math::Vector2 nextStep = calculatePath(npc->getMoveToPoint(),npc);
					if (nextStep.x == 0.0f && nextStep.y == 0.0f)
					{
						npc->setMoveDirection(math::Vector2::calculateUnitVector(npc->getSpritePosition() - npc->getMoveToPoint()));
					}
					else
					{
						npc->setMoveDirection(math::Vector2(nextStep.x, nextStep.y));
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

				//clear clipping
				for (activeObject gameObjectOther : allObjects)
				{
					gameObject2 = (objects::GameObject*)gameObjectOther._object;
					if(gameObject1 != gameObject2)
					while (gameObject1->isHit((objects::Hitbox) * gameObject2))
					{
						gameObject1->getSprite()->move(math::Vector2::calculateUnitVector(gameObject1->getSpritePosition() - gameObject2->getSpritePosition()) * 0.001);
					}
				}
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


	math::Vector2 LevelAssetManager::calculatePath(const math::Vector2& goal,const objects::NPC* npc)
	{
		//varijacija A* algoritma
		std::vector<math::Vector3> endpoints;
		std::vector<math::Vector3> path;
		std::vector<math::Vector3> passed;
		std::vector<math::Vector2> directionsAll = objects::NPC::directionsAll;
		for (math::Vector2 direction : directionsAll)
		{
			direction = direction * STEPDISTANCE;
		}

		std::vector<math::Vector2> splits;

		if (goal.distanceFrom(npc->getSpritePosition()) < STEPDISTANCE)
			return math::Vector2(0, 0);

		endpoints.push_back(math::Vector3(npc->getSpritePosition().x, npc->getSpritePosition().y, 0));
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
			passed.push_back(endpointCurrent);

			math::Vector2 currentPosition = math::Vector2(endpointCurrent.x, endpointCurrent.y);

			//if endpoint near end return first step;
			float smallestDistance = currentPosition.distanceFrom(goal);
			if (smallestDistance < STEPDISTANCE)
			{
				std::sort(passed.begin(), passed.end(), [](const math::Vector3& x, const math::Vector3& y) {return x.z > y.z;}); //order most steps to least
				math::Vector3 currentPath = endpointCurrent;
				path.push_back(currentPath);
				for (math::Vector3 point : passed)
				{
					if (point.z == currentPath.z - 1)
					{
						math::Vector2 vectorDistance = math::Vector2(point.x, point.y) - math::Vector2(currentPath.x, currentPath.y);
						vectorDistance.x = abs(vectorDistance.x);
						vectorDistance.y = abs(vectorDistance.y);
						if (vectorDistance.x <= STEPDISTANCE && vectorDistance.y <= STEPDISTANCE)
						{
							currentPath = point;
							path.push_back(currentPath);
						}
					}
				}
				if (path.size() > 1)
				{
					path.pop_back();
				}
				math::Vector3 to;
				if (!path.empty())
				{
					 to = path.back();
				}
				else to = endpointCurrent;
					return math::Vector2::calculateUnitVector(math::Vector2(to.x,to.y) - npc->getSpritePosition());
			}


			//if shorter path_from exists, lower endpoint steps
			for (math::Vector2 direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				for (math::Vector3& pathPoint : passed)
					if (pathPoint.x == nextPositionTemp.x && pathPoint.y == nextPositionTemp.y && pathPoint.z + 1 < endpointCurrent.z)
					{
						endpointCurrent.z = pathPoint.z + 1;
					}
			}

			passed.back().z = endpointCurrent.z;

			//only directions to unpassed and unblocked points
			for (math::Vector2 direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				if (!pathBlocked(nextPositionTemp, currentPosition,npc) && !std::any_of(passed.begin(), passed.end(), [&](const math::Vector3& x)
					{return x.x == nextPositionTemp.x && x.y == nextPositionTemp.y; }))
					directions.push_back(direction);
			}

			//get smallest distance from available directions
			bool multipleEndpoints = false;
			for (math::Vector2 direction : directions)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				float distance = nextPositionTemp.distanceFrom(goal);
				if (distance < smallestDistance || direction == directions.front())
				{
					smallestDistance = distance;
				}
			}

			//move endpoint to next, if more than 1 create additional

			for (math::Vector2 direction : directions)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * STEPDISTANCE;
				float distance = nextPositionTemp.distanceFrom(goal);
				if (distance == smallestDistance)
				{
					if (!multipleEndpoints)
					{
						multipleEndpoints = true;
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

			directions.clear();
		}
	}

	bool LevelAssetManager::pathBlocked(const math::Vector2& positionTo, const math::Vector2& current, const objects::NPC* npc)
	{
		math::Vector2 distanceVector = positionTo - current;
		objects::Hitbox temp(current, npc->getCollisionRange());
		for (activeObject gameObject : _gameObjects)
		{
			const objects::GameObject& gameObject1 = *(objects::GameObject*) gameObject._object;
			if (gameObject1.willBeHit(temp, distanceVector))
				return true;
		}

		for (activeObject NPC : _NPCs)
		{
			if (NPC._object != npc)
			{
				const objects::GameObject& gameObject1 = *(objects::GameObject*) NPC._object;
				if (gameObject1.willBeHit(temp, distanceVector))
					return true;
			}
		}

		if (_player->willBeHit(temp, distanceVector))
			return true;
		
		return false;
	}
}