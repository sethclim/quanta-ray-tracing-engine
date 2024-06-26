#include "Application.hpp"

void Application::Run()
{
    double t = 0.0;

    const double fpsLimit = 1.0 / 60.0;

    while (glfwGetKey(WindowController::GetInstance().GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(WindowController::GetInstance().GetWindow()) == 0)
    {
        // PROCESS INPUT
        double newTime = glfwGetTime();
        double frameTime = newTime - m_LastFrameTime;
        m_LastFrameTime = newTime;

        float deltaTime = std::min(frameTime, fpsLimit);

        /*     m_gameController->ProcessInput(deltaTime);*/
        // m_gameController->Update(deltaTime);

        // Utilities::FPSCounter::CalculateFrameRate();

        // RENDER
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*      m_gameController->Render();*/

        glfwSwapBuffers(WindowController::GetInstance().GetWindow());

        glfwPollEvents();
    }

    // m_gameController->CleanUp();
}