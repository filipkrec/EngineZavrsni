#pragma once
#include "../Physics/Objects/NPC.h"
#include <thread>


namespace engine {
	struct NPCThread
	{
		std::thread* _thread;
		objects::NPC* _npc;
		bool _isFinished;
		bool _returnValue;

		NPCThread() : _thread(nullptr), _npc(nullptr), _isFinished(false), _returnValue(false) {};
	};
}