#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include "quanta.hpp"

#include "pugixml.hpp"

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void SaveScene(Scene::SceneGraph scene);
};

#endif
