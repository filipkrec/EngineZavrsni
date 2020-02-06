#include "LevelAssetManager.h"


std::vector<LevelAssetManager::activeObject> LevelAssetManager::_sprites;
std::vector<LevelAssetManager::activeObject> LevelAssetManager::_gameObjects;
std::vector<LevelAssetManager::activeObject> LevelAssetManager::_NPCs;
std::vector<LevelAssetManager::activeObject> LevelAssetManager::_labels;
physics::Player* LevelAssetManager::_player = nullptr;

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

void LevelAssetManager::add(physics::GameObject* gameObject, const std::string& name)
{
	_gameObjects.push_back(activeObject((void*)gameObject, name));
}

void LevelAssetManager::cleanGameObjects()
{
	for (activeObject gameObject : _gameObjects)
		delete (physics::GameObject*)gameObject._object;

	_sprites.clear();
}

physics::GameObject* LevelAssetManager::getGameObject(const std::string& name)
{
	for (activeObject gameObject : _gameObjects)
	{
		if (gameObject._name == name)
			return (physics::GameObject*)gameObject._object;
	}
	return nullptr;
}

void LevelAssetManager::add(physics::NPC* NPC, const std::string& name)
{
	_NPCs.push_back(activeObject((void*)NPC, name));
}

void LevelAssetManager::cleanNPCs()
{
	for (activeObject NPC : _NPCs)
		delete (physics::NPC*)NPC._object;

	_sprites.clear();
}

physics::NPC* LevelAssetManager::getNPC(const std::string& name)
{
	for (activeObject npc : _gameObjects)
	{
		if (npc._name == name)
			return (physics::NPC*)npc._object;
	}
	return nullptr;
}


void LevelAssetManager::addToLayer(graphics::Layer* layer)
{
	for (activeObject sprite : _sprites)
	{
		layer->add((graphics::Sprite*)sprite._object);
	}
	for (activeObject gameObject : _gameObjects)
	{
		graphics::Sprite* temp = &(((physics::GameObject*)gameObject._object)->_sprite);
		layer->add(temp);
	}

	for (activeObject npc : _NPCs)
	{
		layer->add(&(((physics::NPC*)npc._object)->_sprite));
	}

	for (activeObject label : _labels)
	{
		layer->add((graphics::Label*)label._object);
	}

	if (_player != nullptr)
		layer->add(&(_player->_sprite));
}


void LevelAssetManager::clean()
{
	cleanGameObjects();
	cleanNPCs();
	cleanSprites();
	cleanLabels();
}

