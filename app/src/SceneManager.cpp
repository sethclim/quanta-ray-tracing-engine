#include "SceneManager.hpp"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

// Scene::SceneGraph scene
void SceneManager::SaveScene()
{
    // get a test document
    pugi::xml_document doc;
    doc.load_string("<foo bar='baz'>hey</foo>");

    auto folder_path = std::filesystem::path("../../app/src/scene/");
    auto folder_abs = std::filesystem::absolute(folder_path);

    std::ofstream file(folder_abs.string() + "save_file_output.xml");

    doc.save(file);

    file.close();

    if (file.fail()) {
        std::cerr << "Failed to save XML to file." << std::endl;
    }
}