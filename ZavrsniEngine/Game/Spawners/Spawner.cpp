#include "Spawner.h"

Spawner::Spawner(const objects::NPC& prototype, const math::Vector2& spawnLocation, const math::Vector2& destination, float spawnTime, unsigned int id)
	:_prototype(prototype), _spawnLocation(spawnLocation), _destination(destination), _spawnTime(spawnTime),_spawnerId(id),_currentId(0)
{
}

objects::NPC* Spawner::Spawn()
{
	if (_timer.elapsed() >= _spawnTime &&  _currentId < 20)
	{
		objects::NPC* temp = new objects::NPC(_prototype);
		temp->setPosition(_spawnLocation);
		temp->setMoveToPoint(_destination);
		temp->DoNotDestroySprite();
		_timer.reset();
		_currentId++;
		return temp;
	}
	else return nullptr;
}


std::string Spawner::getSpawnName()
{
	return std::to_string(_spawnerId) + ':' + std::to_string(_currentId - 1);
}


void Spawner::setSpawnLocation(const math::Vector2& spawnLocation)
{
	_spawnLocation = spawnLocation;
}

void Spawner::setDestination(const math::Vector2& destination)
{
	_destination = destination;
}