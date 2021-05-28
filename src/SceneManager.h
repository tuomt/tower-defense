#pragma once
#include <memory>
#include "Scene.h"

class SceneManager
{
public:
    std::unique_ptr<Scene> currentScene;

	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	}

private:
	SceneManager() {}
	//SceneManager(SceneManager const&);
public:
	// Prevent making a copy of the singleton
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
};

