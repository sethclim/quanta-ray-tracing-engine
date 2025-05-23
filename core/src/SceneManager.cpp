#include "SceneManager.hpp"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

/// <summary>
/// Saves SceneGraph into XML scene format
/// </summary>
/// <param name="scene"></param>
void SceneManager::SaveScene(Scene::SceneGraph scene)
{
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
            object.append_attribute("id") = sphere->id;
            pugi::xml_node position = object.append_child("position");
            position.append_attribute("x") = sphere->Origin.x;
            position.append_attribute("y") = sphere->Origin.y;
            position.append_attribute("z") = sphere->Origin.z;
            pugi::xml_node radius = object.append_child("radius");
            radius.append_attribute("value") = sphere->Radius;

            pugi::xml_node material = object.append_child("material");
            material.append_attribute("value") = sphere->Material->GetName().c_str();
        }
    }

    for (uint32_t i = 0; i < scene.materials.size(); i++)
    {
        std::shared_ptr<Materials::Material> mat = scene.materials[i];
        pugi::xml_node material = materials.append_child("material");
        material.append_attribute("name") = mat.get()->GetName().c_str();
        pugi::xml_node color = material.append_child("color");
        color.append_attribute("r") = mat.get()->Albedo.x;
        color.append_attribute("g") = mat.get()->Albedo.y;
        color.append_attribute("b") = mat.get()->Albedo.z;

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
                material.append_attribute("fuzz") = metal->fuzz;
            }
            else
            {
                auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(mat);
                material.append_attribute("type") = "Emissive";

                pugi::xml_node emission_strength = material.append_child("emission_strength");
                emission_strength.append_attribute("value") = emissive.get()->EmissionStrength;

                pugi::xml_node emission_color = material.append_child("emission_color");
                emission_color.append_attribute("r") = emissive.get()->EmissionColor.x;
                emission_color.append_attribute("g") = emissive.get()->EmissionColor.y;
                emission_color.append_attribute("b") = emissive.get()->EmissionColor.z;
            }
        }
    }

    auto folder_path = std::filesystem::path("../../core/src/scene/");
    auto folder_abs = std::filesystem::absolute(folder_path);

    std::ofstream file(folder_abs.string() + "default_scene.xml");

    doc.save(file);

    file.close();

    if (file.fail())
    {
        std::cerr << "Failed to save XML to file." << std::endl;
    }
}

/// <summary>
/// Loads a scene from XML into SceneGraph
/// </summary>
/// <param name="scene"></param>
void SceneManager::LoadScene(Scene::SceneGraph &scene, std::string name)
{
    scene.ray_targets.clear();
    scene.materials.clear();

    pugi::xml_document doc;

    auto folder_path = std::filesystem::path("../../core/src/scene/");
    auto folder_abs = std::filesystem::absolute(folder_path);

    std::ifstream file(folder_abs.string() + name);

    doc.load(file);

    std::cout << "Loaded XML" << std::endl;

    for (pugi::xml_node scenexml = doc.first_child(); scenexml; scenexml = scenexml.next_sibling())
    {
        std::cout << "Parsing Scene..." << std::endl;

        pugi::xml_node objects = scenexml.first_child();
        pugi::xml_node materials = objects.next_sibling();

        std::cout << "Materials--------------------------------" << std::endl;
        for (pugi::xml_node material = materials.first_child(); material; material = material.next_sibling())
        {
            pugi::xml_attribute name = material.first_attribute();
            std::cout << "----" << name.value() << "----" << std::endl;

            pugi::xml_attribute type = name.next_attribute();
            std::cout << type.name() << "=" << type.value() << std::endl;

            pugi::xml_node color = material.first_child();
            pugi::xml_attribute r = color.first_attribute();
            pugi::xml_attribute g = r.next_attribute();
            pugi::xml_attribute b = g.next_attribute();

            std::cout << "color [" << r.name() << ":" << r.value() << " " << g.name() << ":" << g.value() << " " << b.name() << ":" << b.value() << "]" << std::endl;

            float r_fl = std::strtof(r.value(), nullptr);
            float g_fl = std::strtof(g.value(), nullptr);
            float b_fl = std::strtof(b.value(), nullptr);

            if (std::strcmp(type.value(), "Lambertian") == 0)
            {
                Materials::Lambertian lambertian = Materials::Lambertian(name.value());
                lambertian.Albedo = Math::Vector3<float>(r_fl, g_fl, b_fl);
                // lambertian.EmissionStrength = emission_strength_fl;
                // lambertian.EmissionColor = Math::Vector3<float>(emission_r_fl, emission_g_fl, emission_b_fl);

                scene.materials.push_back(std::make_shared<Materials::Lambertian>(lambertian));
            }
            else if (std::strcmp(type.value(), "Metal") == 0)
            {
                pugi::xml_attribute fuzz = type.next_attribute();
                std::cout << fuzz.name() << "=" << fuzz.value() << std::endl;

                float fuzz_fl = std::strtof(fuzz.value(), nullptr);

                Materials::Metal metal = Materials::Metal(name.value(), {r_fl, g_fl, b_fl}, fuzz_fl);
                // metal.EmissionStrength = emission_strength_fl;
                // metal.EmissionColor = Math::Vector3<float>(emission_r_fl, emission_g_fl, emission_b_fl);

                scene.materials.push_back(std::make_shared<Materials::Metal>(metal));
            }
            else if (std::strcmp(type.value(), "Emissive") == 0)
            {
                pugi::xml_node emission_strength = color.next_sibling();
                pugi::xml_attribute emission_strength_value = emission_strength.first_attribute();
                std::cout << "emission strength " << emission_strength_value.value() << std::endl;

                pugi::xml_node emission_color = emission_strength.next_sibling();
                pugi::xml_attribute emission_r = emission_color.first_attribute();
                pugi::xml_attribute emission_g = emission_r.next_attribute();
                pugi::xml_attribute emission_b = emission_g.next_attribute();

                std::cout << "emission color [" << emission_r.name() << ":" << emission_r.value() << emission_g.name() << ":" << emission_g.value() << emission_b.name() << ":" << emission_b.value() << "]" << std::endl;
                float emission_strength_fl = std::strtof(emission_strength_value.value(), nullptr);
                float emission_r_fl = std::strtof(emission_r.value(), nullptr);
                float emission_g_fl = std::strtof(emission_g.value(), nullptr);
                float emission_b_fl = std::strtof(emission_b.value(), nullptr);

                Materials::Emissive light = Materials::Emissive(name.value());
                light.Albedo = Math::Vector3<float>(r_fl, g_fl, b_fl);
                light.EmissionStrength = emission_strength_fl;
                light.EmissionColor = Math::Vector3<float>(emission_r_fl, emission_g_fl, emission_b_fl);

                scene.materials.push_back(std::make_shared<Materials::Emissive>(light));
            }
            else
            {
                std::cout << "Unknown Material" << std::endl;
            }
        }

        std::cout << "Objects--------------------------------" << std::endl;
        for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
        {
            pugi::xml_attribute name = object.first_attribute();
            std::cout << "----" << name.value() << "----" << std::endl;

            pugi::xml_attribute type = name.next_attribute();
            std::cout << type.name() << "=" << type.value() << std::endl;
            pugi::xml_attribute id = type.next_attribute();
            std::cout << id.name() << "=" << id.value() << std::endl;

            int identifier = std::stoi(id.value(), nullptr);

            pugi::xml_node position = object.first_child();
            pugi::xml_attribute x = position.first_attribute();
            pugi::xml_attribute y = x.next_attribute();
            pugi::xml_attribute z = y.next_attribute();

            std::cout << "position [" << x.name() << ":" << x.value() << " " << y.name() << ":" << y.value() << " " << z.name() << ":" << z.value() << "]" << std::endl;

            std::unordered_map<std::string, size_t> nameToIndex;
            for (size_t i = 0; i < scene.materials.size(); ++i)
            {
                nameToIndex[scene.materials[i].get()->GetName()] = i;
            }

            if (std::strcmp(type.value(), "Sphere") == 0)
            {
                pugi::xml_node radius = position.next_sibling();
                pugi::xml_attribute radius_value = radius.first_attribute();
                std::cout << radius_value.name() << "=" << radius_value.value() << std::endl;

                pugi::xml_node material = radius.next_sibling();
                pugi::xml_attribute material_value = material.first_attribute();
                std::cout << material_value.name() << "=" << material_value.value() << std::endl;

                std::string material_name = material_value.value();

                Scene::Shapes::Sphere sphere;
                float x_fl = std::strtof(x.value(), nullptr);
                float y_fl = std::strtof(y.value(), nullptr);
                float z_fl = std::strtof(z.value(), nullptr);
                sphere.Origin = Math::Vector3<float>(x_fl, y_fl, z_fl);
                sphere.id = identifier;

                auto it = nameToIndex.find(material_name);

                if (it != nameToIndex.end())
                {
                    size_t index = it->second;
                    std::cout << material_name << " found at material index: " << index << "\n";
                    sphere.Material = scene.materials[index];
                }
                else
                {
                    std::cout << "Material Not found\n";
                }

                float radius_fl = std::strtof(radius_value.value(), nullptr);
                sphere.Radius = radius_fl;
                // sphere.id = 666;

                scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere));
            }
        }
    }
}