#pragma once
#include <vector>
#include "../Graphics/Label.h"
#include "../Physics/Physics.h"
#include "../Graphics/Layer.h"

class LevelAssetManager
{
public:
	struct activeObject {
		void* _object;
		std::string _name;

		activeObject(void* object, const std::string& name)
		{
			_object = object;
			_name = name;
		}

		activeObject(const activeObject& object)
		{
			_object = object._object;
			_name = object._name;
		}
	};
private:
	static std::vector<activeObject> _sprites;
	static std::vector<activeObject> _gameObjects;
	static std::vector<activeObject> _NPCs;
	static std::vector<activeObject> _labels;
	static physics::Player* _player;
public:
	static void add(graphics::Sprite* sprite, const std::string& name);
	static void cleanSprites();
	static graphics::Sprite* getSprite(const std::string& name);


	static void add(graphics::Label* label, const std::string& name);
	static void cleanLabels();
	static graphics::Label* getLabel(const std::string& name);

	static void add(physics::GameObject* gameeObject, const std::string& name);
	static void cleanGameObjects();
	static physics::GameObject* getGameObject(const std::string& name);

	static void add(physics::NPC* NPC, const std::string& name);
	static void cleanNPCs();
	static physics::NPC* getNPC(const std::string& name);

	static void addToLayer(graphics::Layer* layer);
	static void clean();

	inline physics::Player* getPlayer() const { return _player; }
private:
	LevelAssetManager() {};
};