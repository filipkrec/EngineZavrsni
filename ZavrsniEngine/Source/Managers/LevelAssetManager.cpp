#include "LevelAssetManager.h"
#define ASTEPDISTANCE 0.5f
#define AFIDELITY 400


namespace lam {
	graphics::Layer* LevelAssetManager::_layer;
	engine::Timer* LevelAssetManager::_timer = new engine::Timer();

	std::vector<engine::NPCThread> LevelAssetManager::_threads;

	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_sprites;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_gameObjects;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_NPCs;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_labels;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_pickups;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_allObjects;
	std::vector<LevelAssetManager::activeObject> LevelAssetManager::_allActors;
	objects::Player* LevelAssetManager::_player = nullptr;

	std::vector<LevelAssetManager::UIElement> LevelAssetManager::_UIElements;
	std::vector<graphics::Line*> LevelAssetManager::_shots;

	void LevelAssetManager::processBegin(engine::Window& window)
	{
			processPathfinding();
			processSight();
			processPlayer(window);
			processNPCs(window);
			processCollision();
			processHitDetection();
			processMovement();
			processUI();
			refreshWeapons();
	}

	void LevelAssetManager::fillObjects()
	{
		_allActors.clear();
		_allObjects.clear();

		objects::GameObject* playerObject = (objects::GameObject*)_player;

		if (_player != nullptr)
		_allActors.push_back(activeObject((void*)playerObject, "Player"));

		_allActors.insert(_allActors.end(), _NPCs.begin(), _NPCs.end());

		if (_player != nullptr)
			_allObjects.push_back(activeObject((void*)playerObject, "Player"));

		_allObjects.insert(_allObjects.end(), _gameObjects.begin(), _gameObjects.end());
		_allObjects.insert(_allObjects.end(), _NPCs.begin(), _NPCs.end());

	}

	void LevelAssetManager::processSight()
	{
		for (activeObject& actor : _allActors)
		{
			objects::Actor* actor1 = (objects::Actor*) actor._object;
			if (actor1 != nullptr)
			{
				for (activeObject& gameObject : _allObjects)
				{
					objects::GameObject* gameObject1 = (objects::GameObject*)gameObject._object;
					if (actor1 != gameObject1)
					{
						if (actor1->objectIsInSight(*gameObject1))
							actor1->addSighted(gameObject1);
					}
				}
				actor1->clearObstructedSighted();
			}
		}
	}

	void LevelAssetManager::processPlayer(engine::Window& window)
	{
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
	}

	void LevelAssetManager::processPathfinding()
	{
		for (activeObject& NPC : _NPCs)
		{
			objects::NPC* npc = (objects::NPC*)NPC._object;

			if (!npc->isPatroling() && npc->isPatrol())
			{
				npc->togglePatroling();
				npc->setMoveToPoint(npc->getPatrolEndPoint());
				npc->setPatrolOriginPoint(npc->getPosition());
			}
			if (npc->isPatroling() && !npc->isPatrol())
			{
				npc->togglePatroling();
			}

			if (npc->getPosition().distanceFrom(npc->getMoveToPoint()) <= ASTEPDISTANCE)
			{
				if (!npc->isPointReached())
				{
					npc->togglePointReached();
				}
			}

			if (!npc->isPointReached())
			{
				if (npc->isPathFinished())
				{
					math::Vector2 nextPath = npc->getCheckpoint();
					if (npc->getPosition().distanceFrom(nextPath) <= ASTEPDISTANCE)
					{
						if (npc->getPosition().distanceFrom(nextPath) <= ASTEPDISTANCE)
						{
							npc->popPath();
						}
						nextPath = npc->getCheckpoint();
					}
					math::Vector2 temp = npc->getPosition();
					if (abs(temp.x) < 0.02)
					{
						temp.x = ((temp.x > 0) - (temp.x < 0)) * 0.02;
						//sign func  (temp.x > 0) - (temp.x < 0)
					}
					if (abs(temp.y) < 0.02)
					{
						temp.y = ((temp.y > 0) - (temp.y < 0)) * 0.02;
						//sign func  (temp.x > 0) - (temp.x < 0)
					}
					math::Vector2 direction = math::Vector2::calculateUnitVector(nextPath - npc->getPosition());

					if (direction == math::Vector2(0, 0) && npc->getPosition().distanceFrom(npc->getMoveToPoint()) >= ASTEPDISTANCE)
						direction = math::Vector2::calculateUnitVector(npc->getMoveToPoint() - npc->getPosition());
					
					npc->setMoveDirection(direction);
				}

				if (npc->seekPath())
				{
					std::vector<engine::NPCThread>::iterator it =
						std::find_if(_threads.begin(), _threads.end(), [&](const engine::NPCThread& x) { return x._npc == npc; });

					if(it == _threads.end())
					for (engine::NPCThread& thread : _threads)
					{
						if (thread._npc == nullptr)
						{
							thread._npc = npc;
							thread._thread = new std::thread(LevelAssetManager::threadFunction, npc,
								std::ref(thread._isFinished));
							npc->toggleSeekPath();
							break;
						}
					}
					else
					{
						if ((*it)._isFinished == true)
						{
							(*it)._thread->join();
							delete (*it)._thread;
							(*it)._npc = nullptr;
							(*it)._isFinished = false;
						}
					}
				}
			}
			else
			{
				std::vector<engine::NPCThread>::iterator it =
					std::find_if(_threads.begin(), _threads.end(), [&](const engine::NPCThread& x) { return x._npc == npc; });

				if (it != _threads.end())
				{
					(*it)._thread->join();
					delete (*it)._thread;
					(*it)._thread = nullptr;
					(*it)._npc = nullptr;
					(*it)._isFinished = false;
				}

				npc->setMoveDirection(math::Vector2(0.0f, 0.0f));
				if (npc->isPatroling())
				{
					npc->setMoveToPoint(npc->getMoveToPoint() == npc->getPatrolEndPoint() ? npc->getPatrolOriginPoint() : npc->getPatrolEndPoint());
				}
			}
		}
	}

	void LevelAssetManager::processNPCs(engine::Window& window)
	{
		for (activeObject& npc : _NPCs)
		{
			((objects::NPC*)npc._object)->process(window);
		}
	}

	void LevelAssetManager::processCollision()
	{
		//colission
		objects::GameObject* gameObject1;
		objects::GameObject* gameObject2;
		for (activeObject& gameObject : _allObjects)
		{
			gameObject1 = (objects::GameObject*)gameObject._object;

			if (gameObject1->getAllegiance() != objects::Allegiance::ENVIROMENT)
			{
				gameObject1->calculateNextMove();
				gameObject1->savePreviousForce();
			}
		}

		for (activeObject& gameObject : _allObjects)
		{
			gameObject1 = (objects::GameObject*)gameObject._object;

			if (!gameObject1->isCollsionOn() || gameObject1->getAllegiance() == objects::Allegiance::ENVIROMENT) //ne racunaj koliziju za enviroment
				continue; 

			for (activeObject& gameObjectOther : _allObjects)
			{
				gameObject2 = (objects::GameObject*)gameObjectOther._object;

				objects::Hitbox* hitbox1 = (objects::Hitbox*)gameObject1;
				if (gameObject2->isCollsionOn() && gameObject1 != gameObject2)
				{
					if (gameObject2->willBeHit(*hitbox1, gameObject1->_nextMove))
					{
						gameObject1->collide(*gameObject2);

						if(gameObject2->getAllegiance() == objects::Allegiance::ENVIROMENT) //ako je enviroment racunaj povratnu silu
							gameObject2->collide(*gameObject1); 
					}
				}
			}
		}
	}

	void LevelAssetManager::processHitDetection()
	{
		for (activeObject& actor : _allActors)
		{
			objects::Actor* currentActor = (objects::Actor*)actor._object;
			if (currentActor->getWeapon() != nullptr)
			{
				for (math::Vector2 firedShot : currentActor->getWeapon()->_firedShots)
				{
					std::vector<std::pair<objects::GameObject*, math::Vector2>> shotObjects;
					for (activeObject& gameObject : _allObjects)
					{
						objects::GameObject* gameObject1 = (objects::GameObject*)gameObject._object;
						if (gameObject1->isCollsionOn() && gameObject1 != currentActor && gameObject1->getLineIntersection(currentActor->getWeapon()->getShotPosition(), firedShot) != math::Vector4(0, 0, 0, 0))
						{
							math::Vector4 clipPoints = gameObject1->getLineIntersection(currentActor->getWeapon()->getShotPosition(), firedShot);
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
						graphics::Line* lineSprite = new graphics::Line(math::Vector2(currentActor->getWeapon()->getShotPosition()), math::Vector2(hitPoint));
						_shots.push_back(lineSprite);
						math::Vector2 unitVector = hitPoint - currentActor->getWeapon()->getShotPosition();
						unitVector = math::Vector2::calculateUnitVector(unitVector);
						currentActor->getWeapon()->onShot(closestShot.first);
						closestShot.first->onHit(currentActor->getWeapon());
						closestShot.first->calculateColission(math::Vector3(unitVector.x, unitVector.y, currentActor->getWeapon()->getForce()));
						shotObjects.clear();
					}
					else
					{
						graphics::Line* lineSprite = new graphics::Line(math::Vector2(currentActor->getWeapon()->getShotPosition()), firedShot);
						_shots.push_back(lineSprite);
					}
				}
			}
		}
	}

	void LevelAssetManager::processMovement()
	{
		objects::GameObject* gameObject1;
		objects::GameObject* gameObject2;
		for (activeObject& gameObject : _allObjects)
		{
			gameObject1 = (objects::GameObject*)gameObject._object;

			if (gameObject1->getAllegiance() == objects::Allegiance::ENVIROMENT)
				continue;

			gameObject1->move();

			//clear clipping
			if(gameObject1->isCollsionOn() && gameObject1->getCurrentForce().z != 0)
			for (activeObject& gameObjectOther : _allObjects)
			{
				gameObject2 = (objects::GameObject*)gameObjectOther._object;
				if (gameObject2->isCollsionOn() && gameObject1 != gameObject2)
					while (gameObject1->isHit((objects::Hitbox&) *gameObject2))
					{
						gameObject1->movePosition(math::Vector2::calculateUnitVector(gameObject1->getPosition() - gameObject2->getPosition()) * 0.01);
					}
			}
		}
	}
	
	void LevelAssetManager::processUI()
	{
		for (UIElement element : _UIElements)
		{
				element._sprite->setPosition(math::Vector2(0,0) - Camera::getInstance()->getOffset() + element._UIPosition);
		}
	}



	void LevelAssetManager::refreshWeapons()
	{
		objects::Actor* actor;
		for (activeObject& currentActor : _allActors)
		{
			actor = (objects::Actor*)currentActor._object;
			if (!actor->hasSwitchedWeapon())
				continue;

			actor->toggleSwitchedWeapon();

			std::vector<activeObject>::iterator it = std::find_if(_sprites.begin(), _sprites.end(), [&](const activeObject& x)
				{
					return x._name.find(currentActor._name + "Weapon") != std::string::npos;
				});

			if (actor->getWeapon() != nullptr)
			{
				if (it == _sprites.end())
				{
					add(actor->getWeapon(), currentActor._name + "Weapon" + std::to_string(actor->getWeapon()->getID()));
				}
				else
				{
					if ((*it)._name != currentActor._name + "Weapon" + std::to_string(actor->getWeapon()->getID()))
					{
						graphics::Sprite* sprite = (graphics::Sprite*)(*it)._object;
						sprite->DestroySprite();
						(*it)._object = actor->getWeapon();
						add(actor->getWeapon(), currentActor._name + "Weapon" + std::to_string(actor->getWeapon()->getID()));
					}
				}
			}
			else if (it != _sprites.end())
			{
				graphics::Sprite* sprite = (graphics::Sprite*)(*it)._object;
				sprite->DestroySprite();
				_sprites.erase(it);
			}
		}
	}

	void LevelAssetManager::threadFunction(objects::NPC* npc, bool& finished)
	{
		finished = false;
		calculatePath(npc->getMoveToPoint(),npc);
		finished = true;
	}


	void LevelAssetManager::processEnd(engine::Window& window)
	{
			for (graphics::Line* shot : _shots)
			{
				if (shot->isNew())
					_layer->add((graphics::Sprite*)shot);
			}

			//cleanup

			for (activeObject& pickup : _pickups)
			{
				objects::Pickup* temp = (objects::Pickup*)pickup._object;
				temp->processTime();
			}

			for (graphics::Line* line : _shots)
			{
				line->tick();
			}

			for (activeObject& npc : _NPCs)
			{
				objects::NPC* temp = (objects::NPC*)npc._object;
				if (temp->getActorTimer() >= 1.0f)
					temp->resetActorTimer();
			}

			if (_player != nullptr)
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
					[](activeObject& x) {
						objects::Pickup* temp = (objects::Pickup*)x._object;
						return temp->toDestroySprite();
					}),
				_pickups.end());

			_sprites.erase(
				std::remove_if(_sprites.begin(), _sprites.end(),
					[](activeObject& x) {
						graphics::Sprite* temp = (graphics::Sprite*)x._object;
						return temp->toDestroySprite();
					}),
				_sprites.end());

			bool fillObj = false;
			_NPCs.erase(
				std::remove_if(_NPCs.begin(), _NPCs.end(),
					[&](activeObject& x) {
						graphics::Sprite* temp = (graphics::Sprite*)x._object;
						if (temp->toDestroySprite())
							fillObj = true;
						return temp->toDestroySprite();
					}),
				_NPCs.end());

			if (fillObj)
				fillObjects();
			cleanUI();
			_timer->reset();
	}

	void LevelAssetManager::init(graphics::Layer* layer)
	{
		while (_threads.size() <= std::thread::hardware_concurrency()) 
		{
			_threads.push_back(engine::NPCThread());
		}
			_layer = layer;
	}

	void LevelAssetManager::clean()
	{
		if (_player != nullptr)
		{
			_player->DestroySprite();
			_player = nullptr;
		}
		cleanGameObjects();
		cleanNPCs();
		cleanSprites();
		cleanPickups();
		cleanLabels();
		cleanUI();
	}

	void LevelAssetManager::setPlayer(objects::Player* player)
	{
		if(_player != nullptr)
		_player->DestroySprite();

		_player = player;
		_player->init();
		_layer->add(_player);

		fillObjects();
	}

	void LevelAssetManager::add(graphics::Sprite* sprite, const std::string& name)
	{
		_sprites.push_back(activeObject((void*)sprite, name));
		_layer->add(sprite);

		fillObjects();
	}

	void LevelAssetManager::addUI(graphics::Sprite* sprite, const std::string& name,const math::Vector2& UIPosition)
	{
		_UIElements.push_back(UIElement(sprite,name, UIPosition));
	}

	void LevelAssetManager::cleanUI()
	{
		_UIElements.erase(
		std::remove_if(_UIElements.begin(), _UIElements.end(), [](UIElement x)
			{
					return x._sprite->toDestroySprite();
			}),_UIElements.end());
	}

	void LevelAssetManager::changeUIElementPosition(const std::string& name, const math::Vector2& UIPosition)
	{
		std::vector<UIElement>::iterator it = std::find_if(_UIElements.begin(), _UIElements.end()
			, [&](UIElement x)
			{
				return x._name == name;
			});

		if (it != _UIElements.end())
			(*it)._UIPosition = UIPosition;
	}



	void LevelAssetManager::cleanSprites()
	{
		for (activeObject& sprite : _sprites)
		{
			graphics::Sprite* temp = (graphics::Sprite*)sprite._object;
			temp->DestroySprite();
		}

		_UIElements.clear();
		_sprites.clear();
	}

	graphics::Sprite* LevelAssetManager::getSprite(const std::string& name)
	{
		for (activeObject& sprite : _sprites)
		{
			if (sprite._name == name)
				return (graphics::Sprite*)sprite._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(graphics::Label* label, const std::string& name)
	{
		_labels.push_back(activeObject((void*)label, name));
		_layer->add(label);
	}

	void LevelAssetManager::cleanLabels()
	{
		for (activeObject& label : _labels)
		{
			graphics::Label* temp = (graphics::Label*)label._object;
			temp->DestroySprite();
		}

		_labels.clear();
	}

	graphics::Label* LevelAssetManager::getLabel(const std::string& name)
	{
		for (activeObject& label : _labels)
		{
			if (label._name == name)
				return (graphics::Label*)label._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(objects::GameObject* gameObject, const std::string& name)
	{
		_gameObjects.push_back(activeObject((void*)gameObject, name));
		_layer->add(gameObject);
		fillObjects();
	}

	void LevelAssetManager::cleanGameObjects()
	{
		for (activeObject& gameObject : _gameObjects)
		{
			objects::GameObject* temp = (objects::GameObject*)gameObject._object;
			temp->DestroySprite();
		}

		_gameObjects.clear();
		fillObjects();
	}

	objects::GameObject* LevelAssetManager::getGameObject(const std::string& name)
	{
		for (activeObject& gameObject : _gameObjects)
		{
			if (gameObject._name == name)
				return (objects::GameObject*)gameObject._object;
		}
		return nullptr;
	}

	void LevelAssetManager::add(objects::NPC* NPC, const std::string& name)
	{
		_NPCs.push_back(activeObject((void*)NPC, name));
		NPC->init();
		_layer->add(NPC);
		fillObjects();
	}

	void LevelAssetManager::cleanNPCs()
	{
		for (activeObject& NPC : _NPCs)
		{
			objects::NPC* temp = (objects::NPC*)NPC._object;
			temp->DestroySprite();
		}

		_NPCs.clear();
		fillObjects();
	}

	objects::NPC* LevelAssetManager::getNPC(const std::string& name)
	{
		for (activeObject& npc : _NPCs)
		{
			if (npc._name == name)
			{
				objects::NPC* npcreturn = (objects::NPC*)npc._object;
				return npcreturn;
			}
		}
		return nullptr;
	}


	objects::NPC* LevelAssetManager::getNPC(const unsigned int id)
	{
		if (id >= _NPCs.size())
			return nullptr;
		else 
		{
				objects::NPC* npcreturn = (objects::NPC*)_NPCs.at(id)._object;
				return npcreturn;
		}
	}


	bool LevelAssetManager::checkForNpcs(const math::Vector2& point)
	{
		objects::NPC* npcp;
		for (activeObject& npc : _NPCs)
		{
			npcp = (objects::NPC*)npc._object;
			if (npcp->isHit(point)) return true;
		}
		return false;
	}

	void LevelAssetManager::add(objects::Pickup* pickup, const std::string& name)
	{
		_pickups.push_back(activeObject((void*)pickup, name));
		_layer->add(pickup);
	}

	void LevelAssetManager::cleanPickups()
	{
		for (activeObject pickup : _pickups)
		{
			objects::Pickup* temp = (objects::Pickup*)pickup._object;
			temp->DestroySprite();
		}

		_pickups.clear();
	}

	objects::Pickup* LevelAssetManager::getPickup(const std::string& name)
	{
		for (activeObject& pickup : _pickups)
		{
			if (pickup._name == name)
				return (objects::Pickup*)pickup._object;
		}
		return nullptr;
	}

	objects::Player* LevelAssetManager::getPlayer()
	{
		if (_player != nullptr)
		return _player;

		return nullptr;
	}

	bool LevelAssetManager::calculatePath(const math::Vector2& goal,objects::NPC* npc)
	{
		//varijacija A* algoritma
		std::vector<math::Vector3> endpoints;
		std::vector<math::Vector3> path;
		std::vector<math::Vector3> passed;
		std::vector<math::Vector2> directionsAll = objects::NPC::directionsAll;
		for (math::Vector2 direction : directionsAll)
		{
			direction = direction * ASTEPDISTANCE;
		}

		std::vector<math::Vector2> splits;

		if (goal.distanceFrom(npc->getPosition()) < ASTEPDISTANCE)
			return true;

		endpoints.push_back(math::Vector3(npc->getPosition().x, npc->getPosition().y, 0));
		// vector3 - X,Y,STEPS
		while (true)
		{
			if (endpoints.empty())
				return true;

			//get endpoint least distance from end with least steps
			std::vector<math::Vector3>::iterator it = min_element(endpoints.begin(), endpoints.end(),
				[&](const math::Vector3& x, const math::Vector3& y)
				{
					float distx = math::Vector2(x.x, x.y).distanceFrom(goal);
					float disty = math::Vector2(y.x, y.y).distanceFrom(goal);
					if (distx == disty)
						return x.z < y.z;
					else return distx < disty;
				});
			math::Vector3& endpointCurrent = *it; //get endpoint
			passed.push_back(endpointCurrent);

			math::Vector2 currentPosition = math::Vector2(endpointCurrent.x, endpointCurrent.y);

			//if endpoint near end return first step;
			float smallestDistance = currentPosition.distanceFrom(goal);

			if (passed.size() > AFIDELITY || smallestDistance < ASTEPDISTANCE)
			{
				std::sort(passed.begin(), passed.end(), [](const math::Vector3& x, const math::Vector3& y) {return x.z > y.z;}); //order most steps to least
				math::Vector3 currentPath = endpointCurrent;
				path.push_back(currentPath);

				if (npc->isPathFinished())
					npc->togglePathFinished();

				for (math::Vector3& point : passed)
				{
					if (point.z == currentPath.z - 1)
					{
						//if within vector distance trace back path to origin
						math::Vector2 vectorDistance = math::Vector2(point.x, point.y) - math::Vector2(currentPath.x, currentPath.y);
						vectorDistance.x = abs(vectorDistance.x);
						vectorDistance.y = abs(vectorDistance.y);
						if (vectorDistance.x <= ASTEPDISTANCE && vectorDistance.y <= ASTEPDISTANCE)
						{
							currentPath = point;
							npc->pushPath(math::Vector2(currentPath.x, currentPath.y));
						}
					}
				}

				if (!npc->isPathFinished())
					npc->togglePathFinished();

				return true;
			}


			//if shorter path_from exists, lower endpoint steps
			for (math::Vector2& direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * ASTEPDISTANCE;
				for (math::Vector3& pathPoint : passed)
					if (pathPoint.x == nextPositionTemp.x && pathPoint.y == nextPositionTemp.y && pathPoint.z + 1 < endpointCurrent.z)
					{
						endpointCurrent.z = pathPoint.z + 1;
					}
			}

			passed.back().z = endpointCurrent.z;

			//only unpassed points
			std::vector<math::Vector2> availablePoints;
			for (math::Vector2& direction : directionsAll)
			{
				math::Vector2 nextPositionTemp = currentPosition + direction * ASTEPDISTANCE;
				if (!std::any_of(passed.begin(), passed.end(), [&](const math::Vector3& x)
					{return x.x == nextPositionTemp.x && x.y == nextPositionTemp.y; }))
					availablePoints.push_back(nextPositionTemp);
			}

			//get smallest distance from available directions
			bool multipleEndpoints = false;
			std::sort(availablePoints.begin(), availablePoints.end(), [&](const math::Vector2& x, const math::Vector2& y)
				{
					return x.distanceFrom(goal) < y.distanceFrom(goal);
				});

			bool end = false;

			for (math::Vector2& point : availablePoints)
			{
				if (end == true)
				{
					if (point.distanceFrom(goal) != math::Vector2(endpoints.back().x, endpoints.back().y).distanceFrom(goal))
						break;
					else if (!pathBlocked(point, currentPosition, npc))
						endpoints.push_back(math::Vector3(point.x, point.y, endpointCurrent.z + 1));
				}
				else if (!pathBlocked(point, currentPosition, npc))
				{
					endpointCurrent.x = point.x;
					endpointCurrent.y = point.y;
					endpointCurrent.z++;
					end = true;
				}
			}

			availablePoints.clear();
		}
	}

	bool LevelAssetManager::pathBlocked(const math::Vector2& positionTo, const math::Vector2& current, const objects::NPC* npc)
	{
		math::Vector2 distanceVector = positionTo - current;
		objects::Hitbox temp(current, npc->getCollisionRange());
		float npcCollisionRange = npc->getCollisionRange().length();

		for (activeObject& gameObject : _allObjects)
		{
			const objects::GameObject& gameObject1 = *(objects::GameObject*) gameObject._object;

			if(gameObject1.getPosition().y == -26)
			std::string name = gameObject._name;

			float gameObjectCollisionRange = gameObject1.getCollisionRange().length();

			if ((gameObject1.getAllegiance() == objects::Allegiance::ENVIROMENT || gameObject1.getWeight() >= npc->getWeight() * 4) &&
				gameObject1.willBeHit(temp, distanceVector)
				)
				return true;
		}

		return false;
	}
}