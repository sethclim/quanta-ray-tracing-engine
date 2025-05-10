#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm> // for std::find_if
#include <cstdlib>
#include <unordered_map>

#include "pugixml.hpp"
#include "quanta.hpp"

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void SaveScene(Scene::SceneGraph scene);
    void LoadScene(Scene::SceneGraph &scene);
};

#endif
