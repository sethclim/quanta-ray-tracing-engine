#include "SceneManager.hpp"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::SaveScene(Scene::SceneGraph scene)
{
    // get a test document
    pugi::xml_document doc;

    pugi::xml_node scenexml = doc.append_child("scene");
    pugi::xml_node objects = scenexml.append_child("objects");
    pugi::xml_node materials = scenexml.append_child("materials");

    for (uint32_t i = 0; i < scene.ray_targets.size(); i++)
    {
        std::shared_ptr<Scene::Shapes::RayTarget> val = scene.ray_targets[i];
        auto sphere = std::dynamic_pointer_cast<Scene::Shapes::Sphere>(val);

        if (sphere)
        {
            pugi::xml_node object = objects.append_child("object");
            object.append_attribute("name") = "Some Sphere";
            object.append_attribute("type") = "Sphere";
            pugi::xml_node position = object.append_child("position");
            position.append_attribute("x") = sphere->Origin.x;
            position.append_attribute("y") = sphere->Origin.y;
            position.append_attribute("z") = sphere->Origin.z;
            pugi::xml_node radius = object.append_child("radius");
            radius.append_attribute("value") = sphere->Radius;
        }
    }

    Math::Vector3<float> Color = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
    float EmissionStrength = 0.0f;
    Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);

    for (uint32_t i = 0; i < scene.materials.size(); i++)
    {
        std::shared_ptr<Materials::Material> mat = scene.materials[i];
        pugi::xml_node material = materials.append_child("material");
        material.append_attribute("name") = mat.get()->GetName().c_str();
        pugi::xml_node color = material.append_child("color");
        color.append_attribute("r") = mat.get()->Color.x;
        color.append_attribute("g") = mat.get()->Color.y;
        color.append_attribute("b") = mat.get()->Color.z;

        pugi::xml_node emission_strength = material.append_child("emission_strength");
        emission_strength.append_attribute("value") = mat.get()->EmissionStrength;

        pugi::xml_node emission_color = material.append_child("emission_color");
        emission_color.append_attribute("r") = mat.get()->EmissionColor.x;
        emission_color.append_attribute("g") = mat.get()->EmissionColor.y;
        emission_color.append_attribute("b") = mat.get()->EmissionColor.z;

        auto lambertian = std::dynamic_pointer_cast<Materials::Lambertian>(mat);
        if (lambertian)
        {
            material.append_attribute("type") = "Lambertian";
        }
        else
        {
            auto metal = std::dynamic_pointer_cast<Materials::Metal>(mat);
            if (metal)
            {
                material.append_attribute("type") = "Metal";
            }
        }
    }

    // param.insert_attribute_after("type", param.attribute("name")) = "float";

    auto folder_path = std::filesystem::path("../../app/src/scene/");
    auto folder_abs = std::filesystem::absolute(folder_path);

    std::ofstream file(folder_abs.string() + "default_scene.xml");

    doc.save(file);

    file.close();

    if (file.fail())
    {
        std::cerr << "Failed to save XML to file." << std::endl;
    }
}