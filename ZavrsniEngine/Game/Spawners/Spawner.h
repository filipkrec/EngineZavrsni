#pragma once
#include "../../Source/Engine.h"

class Spawner
{
	objects::NPC _prototype;
	engine::Timer _timer;
	math::Vector2 _spawnLocation;
	math::Vector2 _destination;
	unsigned int _spawnerId;
	unsigned int _currentId;
	float _spawnTime;

public:
	Spawner();
	Spawner(const objects::NPC& prototype, const math::Vector2& spawnLocation, const math::Vector2& destination, float spawnTime, unsigned int id);
	
	std::string getSpawnName();
	objects::NPC* Spawn();
};