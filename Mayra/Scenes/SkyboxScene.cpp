#include "SkyboxScene.h"
#include "Input.hpp"
#include "Key.hpp"
#include "MouseButton.h"

#include <Window.hpp>

namespace Mayra
{
    SkyboxScene::SkyboxScene()
      : m_ClearColor(glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f))
    {
        m_ScreenSize = glm::vec2(Mayra::Window::Instance()->GetProps()->Width, Mayra::Window::Instance()->GetProps()->Height);
        m_OrthoCamera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        m_Playmode = false;
        m_Projection = glm::perspectiveFov(glm::radians(m_Camera->Zoom), m_ScreenSize.x, m_ScreenSize.y, 0.1f, 10000.0f);

        m_CubeShader = new Mayra::Shader(SHADERS "cubemaps.vert", SHADERS "cubemaps.frag");
        m_SkyboxShader = new Mayra::Shader(SHADERS "skybox.vert", SHADERS "skybox.frag");

        float cubeVertices[] = {
            // positions          // texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        // cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // skybox VAO
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // load textures
        // -------------
        m_CubeTexture = Mayra::Texture2D::LoadFromFile(TEXTURES "wall.jpg", false);

        std::vector<std::string> faces
        {
            TEXTURES "skybox/right.jpg",
            TEXTURES "skybox/left.jpg",
            TEXTURES "skybox/top.jpg",
            TEXTURES "skybox/bottom.jpg",
            TEXTURES "skybox/front.jpg",
            TEXTURES "skybox/back.jpg"
        };

        cubemapTexture = Mayra::TextureCubemap::LoadCubemap(faces);

        // shader configuration
        // --------------------
        m_CubeShader->Bind();
        m_CubeShader->SetInt("texture1", 0);

        m_SkyboxShader->Bind();
        m_SkyboxShader->SetInt("skybox", 0);
    }

    void SkyboxScene::OnUpdate(float deltaTime)
    {
        if (m_Playmode)
        {
            if (Input::Instance()->IsKeyDown(KeyCode::Escape))
            {
                m_Playmode = false;
                glfwSetInputMode(Window::Instance()->Get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Input::Instance()->SetMousePosition(Window::Instance()->GetProps()->GetCenter());
            }
            glm::vec2 offset = Input::Instance()->GetMouseDelta(); // if no movement delta should be zero
            m_Camera->ProcessMouseMovement(offset.x, offset.y);
            m_Camera->ProcessMouseScroll(Mayra::Input::Instance()->GetScrollOffset().y);

            if (Input::Instance()->IsMouseButtonDown(Mayra::MouseButton::Left)) {
                std::cout << "Click!" << std::endl;
            }

            if (Input::Instance()->IsAnyKey(KeyCode::W, KeyCode::Up))
                m_Camera->ProcessKeyboard(FORWARD, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::S, KeyCode::Down))
                m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::A, KeyCode::Left))
                m_Camera->ProcessKeyboard(LEFT, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::D, KeyCode::Right))
                m_Camera->ProcessKeyboard(RIGHT, deltaTime);

            if (Input::Instance()->IsKey(KeyCode::Space))
                m_Camera->ProcessKeyboard(UP, deltaTime);
            if (Input::Instance()->IsKey(KeyCode::X))
                m_Camera->ProcessKeyboard(DOWN, deltaTime);
        }
        
    }

    SkyboxScene::~SkyboxScene()
    {
        // delete all from vector
    }

    void SkyboxScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // draw scene as normal
        m_CubeShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Zoom), m_ScreenSize.x / m_ScreenSize.y, 0.1f, 100.0f);
        m_CubeShader->SetMat4("model", model);
        m_CubeShader->SetMat4("view", view);
        m_CubeShader->SetMat4("projection", projection);

        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_CubeTexture->GetRendererID());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        m_SkyboxShader->Bind();
        view = glm::mat4(glm::mat3(m_Camera->GetViewMatrix())); // remove translation from the view matrix
        m_SkyboxShader->SetMat4("view", view);
        m_SkyboxShader->SetMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }

    void SkyboxScene::OnImGuiRender()
    {
        ImGui::Begin("Skybox");
        if (ImGui::Button("Play")) {
            m_Playmode = true;
            glfwSetInputMode(Mayra::Window::Instance()->Get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Input::Instance()->SetMousePosition(Window::Instance()->GetProps()->GetCenter());
        }

        if (ImGui::Button("Show Fill")) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Line")) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Point")) {
            glPointSize(10.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        ImGui::End();
    }
}
