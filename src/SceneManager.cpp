#include "SceneManager.h"

void SceneManager::enqueueScene(std::unique_ptr<Scene> scene)
{
	// Enqueue a scene to replace the current scene before next update

	if (nextScene == nullptr) {
		nextScene = std::move(scene);
	}
}
