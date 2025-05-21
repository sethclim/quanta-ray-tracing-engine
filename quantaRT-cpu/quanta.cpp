#include "quanta.hpp"

/// <summary>
/// Finds the reflected ray
/// </summary>
/// <param name="N">Normal</param>
/// <param name="Ri">Incident Ray</param>
/// <returns>reflected ray</returns>
Math::Vector3<float> Reflect(Math::Vector3<float> &N, Math::Vector3<float> &Ri)
{
    // Rr = Ri - 2 N (Ri . N)
    return Ri - (N * 2.0f * Ri.Dot(N));
}

void Renderer::SetRenderSettings(uint32_t samplesPerPixel, uint32_t maxBounces, bool accumulate)
{
    _samplesPerPixel = samplesPerPixel;
    _maxBounces = maxBounces;
    _accumulate = accumulate;
}

void Renderer::UpdateImageDimensions(uint32_t width, uint32_t height)
{
    m_Width = width;
    m_Height = height;

    m_ImageHorizontalIter.resize(width);
    m_ImageVerticalIter.resize(height);

    for (uint32_t i = 0; i < width; i++)
        m_ImageHorizontalIter[i] = i;
    for (uint32_t i = 0; i < height; i++)
        m_ImageVerticalIter[i] = i;
}

void Renderer::Render(Math::Vector4<float> *accumulationData, uint32_t *imageData, uint32_t frameIndex)
{
#define MT 1
#if MT
    std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
                  [this, accumulationData, imageData, frameIndex](uint32_t y)
                  {
                      std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
                                    [this, y, accumulationData, imageData, frameIndex](uint32_t x)
                                    {
                                        float flipped_y = m_Height - y;

                                        float normalizedX = (float)x / (float)m_Width;
                                        float normalizedY = (float)flipped_y / (float)m_Height;

                                        uint32_t idx = x + (y * m_Width);

                                        // TODO but debug pixel back in
                                        // bool debug_pixel = false;
                                        // if (item_current == 0)
                                        //     debug_pixel = (x == debug_trace_coord.x && y == debug_trace_coord.y);
                                        // else if (item_current == 1)
                                        //     debug_pixel = idx % 1000;

                                        Math::Vector3<float> color = Math::Vector3<float>(0, 0, 0);

                                        // this was commented out
                                        /*
                                        if (flipped_y < dimensions[1] / 2)
                                            color = Math::Vector3<float>(1, 0, 0);*/

                                        /*	if (x == 535 && y == 318)*/
                                        color = PerPixel(normalizedX, normalizedY, false); // debug_pixel

                                        Math::Vector4<float> finalColor = Math::Vector4<float>(color.x, color.y, color.z, 1.0f);

                                        if (_accumulate)
                                        {
                                            accumulationData[idx] += finalColor;

                                            finalColor = accumulationData[idx];
                                            finalColor /= frameIndex;

                                            // finalColor = m_AccumulationData[x + y * m_Image->GetWidth()];
                                            // finalColor /= (float)m_FrameIndex;
                                        }
                                        /*						else
                                                                {
                                                                    glm::vec4 accumulatedColor = glm::vec4(color.x, color.y, color.z, 1.0f);
                                                                }*/

                                        /*				accumulatedColor = glm::clamp(finalColor, glm::vec4(0.0f), glm::vec4(1.0f));*/

                                        // if (x < dimensions[0] / 2)
                                        //	accumulatedColor = glm::vec4(1, 0, 0, 1);

                                        finalColor = Math::Clamp(finalColor, 0.0f, 1.0f);

                                        imageData[idx] = Utilities::Color::ConvertToRGBA(finalColor);
                                    });
                  });
    // std::cout << "DONE DONE DONE!" << std::endl;
    // useRaytracer = false;
#else

    for (uint32_t y = 0; y < dimensions[1]; y++)
    {
        for (uint32_t x = 0; x < dimensions[0]; x++)
        {

            float flipped_y = dimensions[1] - y;

            float normalizedX = (float)x / (float)dimensions[0];
            float normalizedY = (float)flipped_y / (float)dimensions[1];

            uint32_t idx = x + (y * dimensions[0]);
            bool debug_pixel = false;
            if (item_current == 0)
                debug_pixel = (x == debug_trace_coord.x && y == debug_trace_coord.y);
            else if (item_current == 1)
                debug_pixel = idx % 1000;

            Math::Vector3<float> color = Math::Vector3<float>(0, 0, 0);
            /*
            if (flipped_y < dimensions[1] / 2)
                color = Math::Vector3<float>(1, 0, 0);*/

            /*	if (x == 535 && y == 318)*/
            color = renderer->PerPixel(normalizedX, normalizedY, debug_pixel);

            glm::vec4 finalColor = glm::vec4(color.x, color.y, color.z, 1.0f);

            if (accumulate)
            {
                m_AccumulationData[x + y * m_Image->GetWidth()] += glm::vec4(color.x, color.y, color.z, 1.0f);
                finalColor = m_AccumulationData[x + y * m_Image->GetWidth()];
                finalColor /= (float)m_FrameIndex;
            }
            /*						else
                                    {
                                        glm::vec4 accumulatedColor = glm::vec4(color.x, color.y, color.z, 1.0f);
                                    }*/

            /*				accumulatedColor = glm::clamp(finalColor, glm::vec4(0.0f), glm::vec4(1.0f));*/

            // if (x < dimensions[0] / 2)
            //	accumulatedColor = glm::vec4(1, 0, 0, 1);

            m_ImageData[idx] = Utils::ConvertToRGBA(finalColor);
        }
    }
#endif
}

Math::Vector3<float> Renderer::PerPixel(float image_x, float image_y, bool debug)
{

    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    // if (debug)
    //     std::cout << "coordX " << coordX << "coordY " << coordY << std::endl;

    Math::Vector3<float> camera = Math::Vector3<float>(0.0f, 0.0f, 2.0f);

    // if(debug)
    //     addDebugLine(camera, ray.Direction * 2);

    Math::Vector3<float> pixel_color(0, 0, 0);

    for (int sample = 0; sample < _samplesPerPixel; sample++)
    {
        Ray ray;
        ray.Origin = camera;
        ray.Direction = Math::Vector3<float>(coordX, coordY, -1.0f).Normalize();

        Math::Vector3<float> incomingLight = Math::Vector3<float>(0, 0, 0);
        Math::Vector3<float> rayColor = Math::Vector3<float>(1, 1, 1);

        for (int i = 0; i < _maxBounces; i++)
        {
            HitInfo info{};
            bool res = TraceRay(ray, info, i == 0);

            if (!res)
            {
                if (i == 0)
                    return Math::Vector3<float>(0.337, 0.576, 0.961);

                break;
            }

            if (debug && i > 0 && sample == 0)
                addDebugLine(ray.Origin, info.HitPoint);

            Math::Vector3<float> attenuation;
            Ray ray2;
            if (info.Material->scatter(ray, info, attenuation, ray2))
            {
                // Math::Vector3<float>
                //     emittedLight = info.Material->EmissionColor * info.Material->EmissionStrength;

                rayColor *= attenuation;
                // incomingLight += rayColor * emittedLight;
                /*      incomingLight += rayColor;*/

                ray.Origin = ray2.Origin;
                ray.Direction = ray2.Direction;
            }
            else
            {
                std::shared_ptr<Materials::Material> mat = info.Material;
                auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(mat);

                if (emissive)
                {
                    Math::Vector3<float>
                        emittedLight = emissive->EmissionColor * emissive->EmissionStrength;
                    incomingLight += rayColor * emittedLight;
                    /*         std::cout << "COLOR " << rayColor.ToString() << emittedLight.ToString() << std::endl;*/
                }
                break;
            }
        }

        pixel_color += incomingLight;

        // pixel_color = rayColor;
    }

    return pixel_color / static_cast<float>(_samplesPerPixel);
}

bool Renderer::TraceRay(const Ray &ray, HitInfo &hitinfo, bool isCameraRay)
{
    Utilities::Interval ray_t = Utilities::Interval(0.001, std::numeric_limits<double>::infinity());
    HitInfo closestHit = Miss();

    bool hit = false;

    for (const auto &object : m_Scene.ray_targets)
    {
        HitInfo info{};

        if (!object->hit(ray, ray_t, info))
        {
            continue;
        }
        else
        {
            Materials::Material mat = *info.Material.get();
            if (isCameraRay && mat.GetIsLight())
            {
                continue;
            }
            closestHit = info;
            hit = true;
        }
    }

    hitinfo = closestHit;
    return hit;
}
